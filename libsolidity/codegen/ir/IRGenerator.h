/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * @author Alex Beregszaszi
 * @date 2017
 * Component that translates Solidity code into Yul.
 */

#pragma once

#include <libsolidity/interface/OptimiserSettings.h>
#include <libsolidity/ast/ASTForward.h>
#include <libsolidity/codegen/ir/IRGenerationContext.h>
#include <libsolidity/codegen/YulUtilFunctions.h>
#include <liblangutil/EVMVersion.h>
#include <string>

namespace dev
{
namespace solidity
{

class SourceUnit;

class IRGenerator
{
public:
	IRGenerator(langutil::EVMVersion _evmVersion, OptimiserSettings _optimiserSettings):
		m_evmVersion(_evmVersion),
		m_optimiserSettings(_optimiserSettings),
		m_context(_evmVersion, std::move(_optimiserSettings)),
		m_utils(_evmVersion, m_context.functionCollector())
	{}

	std::string run(ContractDefinition const& _contract);

private:
	class Dispatch;

	std::string generateIR(ContractDefinition const& _contract);
	std::string generateIRFunction(FunctionDefinition const& _function);

	std::string generate(Block const& _block);
	std::string generate(VariableDeclarationStatement const& _variableDeclaration);
	std::string generate(ExpressionStatement const& _exprStatement);

	std::string generate(Assignment const& _assignment);
	std::string generate(BinaryOperation const& _binOp);
	std::string generate(Identifier const& _identifier);

	std::string visitGenerate(ASTNode const& _node);

	std::string constructorCode(FunctionDefinition const& _constructor);
	std::string deployCode(ContractDefinition const& _contract);
	std::string callValueCheck();

	std::string creationObjectName(ContractDefinition const& _contract);
	std::string runtimeObjectName(ContractDefinition const& _contract);

	std::string dispatchRoutine(ContractDefinition const& _contract);

	void resetContext();

	static std::string variableList(size_t _amount, std::string const& _prefix = "v_");

	langutil::EVMVersion const m_evmVersion;
	OptimiserSettings const m_optimiserSettings;

	std::string m_outsourced;
	IRGenerationContext m_context;
	YulUtilFunctions m_utils;
};

}
}
