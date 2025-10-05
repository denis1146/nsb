#include "ProgramOptions.h"

#include <iostream>
#include <array>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string/join.hpp>

namespace {

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
  os << "[";
  std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
  os << "]";
  return os;
}

}

void ProgramOptions::run()
{
  std::cout << std::string(16, '=') + " ProgramOptions " + std::string(16, '=') << std::endl;
  return;

  namespace po = boost::program_options;
  using StringList = std::vector<std::string>;
  constexpr const char optHelp[] = "help";

  try{
    {
      constexpr const char opt1[] = "opt1";
      constexpr const char opt2[] = "opt2";
      constexpr const char opt3[] = "opt3";
      constexpr const char optImplicit[] = "implicit";
      constexpr const char opt5[] = "opt5";
      constexpr const char optList[] = "optList";
      constexpr const char pod[] = "pod";
      constexpr const char optCb[] = "callBack";
      constexpr const char optBool[] = "bool";
      auto cb = [](const auto& v) {
        static bool first{};
        if (!first) {
          std::cout << "From the callback function: " << v << std::endl;
          first = true;
        }
      };
      constexpr std::array args{
        "app_name",
        // "--help",
        "pod0",
        "--opt1=1",
        "--opt2", "2.2",
        "--implicit",
        "-o", "5",
        "--optList=l1",
        "-l l2",
        "-lL+3",
        "-l", "l4",
        "--callBack", "callBack_1",
        "pod1",
        "pod2",
      };
      double op2{};
      int op3{};

      po::options_description desc("Allowed options");
      desc.add_options()
        (optHelp, "produce help message")
        (opt1, po::value<int>(),"option 1")
        (opt2, po::value<double>(&op2),"option 2")
        (opt3, po::value<int>(&op3)->default_value(3),"option 3")
        (optImplicit, po::value<std::string>()->implicit_value("implicit_value")->default_value("default"),"option 4")
        ("opt5,o", po::value<double>()->required(),"option 5")
        ("optList,l", po::value<StringList>()->required(),"optList")
        (pod, po::value<StringList>(),"positional option")
        (optCb, po::value<StringList>()->composing()->notifier(cb), "callback function")
        // (optBool, po::bool_switch(), "bool switch")
      ;

      po::variables_map vm;
      po::store(po::parse_command_line(static_cast<int>(args.size()), args.data(), desc), vm);
      po::notify(vm);

      if (vm.count(optHelp)) {
        std::cout << desc << std::endl;
        return;
      }

      if (vm.count(opt1))
        std::cout << opt1 << " = " << vm[opt1].as<int>() << std::endl;
      if (vm.count(opt2))
        std::cout << opt2 << " = " << vm[opt2].as<double>() << std::endl;
      if (vm.count(opt3))
        std::cout << opt3 << " = " << vm[opt3].as<int>() << " (" << op3 << ")"<< std::endl;
      if (vm.count(optImplicit))
        std::cout << optImplicit << " (implicit_value) = " << vm[optImplicit].as<std::string>() << std::endl;
      if (vm.count(opt5))
        std::cout << opt5 << "(o)" << " = " << vm[opt5].as<double>() << std::endl;
      if (vm.count(optList))
        std::cout << optList << ": " << vm[optList].as<StringList>() << std::endl;
      if (vm.count(optBool))
        std::cout << optBool << ": " << vm[optBool].as<bool>() << std::endl;

      std::cout << "===== positional_options_description =====" << std::endl;
      po::variables_map vmPod;
      po::positional_options_description p;
      p.add(pod, -1)
        .add(optList, 1)      // <<<
      ;
      po::store(po::command_line_parser(static_cast<int>(args.size()), args.data()) 
        .options(desc).positional(p).run(), vmPod); 
      po::notify(vmPod);

      if (vmPod.count(pod))
        std::cout << pod << ": " << vmPod[pod].as<StringList>() <<std::endl;
      if (vmPod.count(optList))
        std::cout << optList << ": " << vmPod[optList].as<StringList>() << std::endl;
    }

    {
      // ===== ProgramOptions [ Multiple Sources ] ===== //
      std::cout << std::endl << "===== Multiple Sources =====" << std::endl;

      constexpr const char optHedden[] = "hidden";
      constexpr const char optIncludes[] = "includes";
      constexpr const char section[] = "section.subsection.key";

      po::options_description hidden("Hidden options");
      hidden.add_options()
        ("hidden", po::value<StringList>(), "Hidden options")
      ;

      po::options_description config("Configuration");
      config.add_options()
        (optIncludes, po::value<StringList>()->composing(), "includes")
        (section, po::value<std::string>(), "section")
      ;

      std::string configFile;
      po::options_description generic("Generic options");
      generic.add_options()
        (optHelp, "produce help message")
        ("config,c", po::value<std::string>(&configFile)->default_value("config.cfg"),
          "name of a file of a configuration.")
      ;

      po::options_description cmdlineOptions;
      cmdlineOptions.add(generic).add(config).add(hidden);
      po::options_description configFileOptions;
      configFileOptions.add(config).add(hidden);
      po::options_description visible("Allowed options");
      visible.add(generic).add(config);

      po::positional_options_description podHidden;
      podHidden.add(optHedden, -1);

      constexpr std::array args2{
        "app_name",
        // "--help",
        // "hidden_0",  
        // "--hidden=hidden_1",
        "-c", "config.cfg",
        "--includes=includes_cmdline",
      };

      po::variables_map vmml;
      store(po::command_line_parser(static_cast<int>(args2.size()), args2.data()).
        options(cmdlineOptions).positional(podHidden).run(), vmml);
      notify(vmml);

      if (vmml.count(optHelp)) {
        std::cout << visible << std::endl;
        return;
      }

      if (!configFile.empty()) {
        std::ifstream ifs(configFile.c_str());
        if (!ifs) {
          std::cout << "can not open config file: " << configFile << std::endl;
          return;
        }
        store(parse_config_file(ifs, configFileOptions), vmml);
        notify(vmml);
      }

      if (vmml.count(optHedden))
        std::cout << optHedden << ": " << vmml[optHedden].as<StringList>() << std::endl;
      if (vmml.count(optIncludes))
        std::cout << optIncludes << ": " << vmml[optIncludes].as<StringList>() << std::endl;
      if (vmml.count(section))
        std::cout << section << ": " << vmml[section].as<std::string>() << std::endl;
    }
  }
  catch(const std::exception& e){
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  catch(...) {
    std::cerr << "Exception of unknown type!\n";
  }
}
