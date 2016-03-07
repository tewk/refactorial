
#include <yaml-cpp/yaml.h>
#include "yaml-util.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/AST.h"
#include <clang/Sema/SemaConsumer.h>
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>
#include "Refactoring.h"

#include <iostream>
#include <fstream>

#include <unistd.h>

using namespace clang;
using namespace std;

#include "Transforms/Transforms.h"

int main(int argc, char **argv)
{	
	string errorMessage("Could not load compilation database");

	// YAML::Node compileCommands = YAML::LoadFile("compile_commands.json");
	
	// vector<YAML::Node> config = YAML::LoadAll(cin);
	// for(auto configSectionIter = config.begin(); configSectionIter != config.end(); ++configSectionIter)
	// {
	// 	TransformRegistry::get().config = YAML::Node();
	// 	//figure out which files we need to work on
	// 	YAML::Node& configSection = *configSectionIter;
	// 	vector<string> inputFiles;
	// 	if(configSection["Files"])
	// 		inputFiles = configSection["Files"].as<vector<string> >();
	// 	else
	// 	{
	// 		llvm::errs() << "Warning: No files selected. Operating on all files.\n";
			
	// 		for(auto iter = compileCommands.begin(); iter != compileCommands.end(); ++iter)
	// 		{
	// 			inputFiles.push_back((*iter)["file"].as<string>());
	// 		}
	// 	}
	// 	if(!configSection["Transforms"])
	// 	{
	// 		llvm::errs() << "No transforms specified in this configuration section:\n";
	// 		llvm::errs() << YAML::Dump(configSection) << "\n";
	// 	}
		
	// }
	vector<string> inputFiles;
	inputFiles.push_back( argv[2] );

	//load up the compilation database
	std::unique_ptr<tooling::CompilationDatabase> Compilations(tooling::CompilationDatabase::loadFromDirectory(".", errorMessage));
	RefactoringTool rt(*Compilations, inputFiles);
		
		
	YAML::Node config;
	YAML::Node data;
	data[ argv[1] ] = argv[2];
	config["MethodMove"] = data;
	TransformRegistry::get().config = config;
	TransformRegistry::get().replacements = &rt.getReplacements();
	rt.run(new TransformFactory(TransformRegistry::get()["MethodMoveTransform"]));
	return 0;
}
