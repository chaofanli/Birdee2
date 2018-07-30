#include <AST.h>
#include <CompileError.h>
#include <assert.h>

namespace Birdee
{
	template <typename T>
	T SetPos(T&& v, SourcePos pos)
	{
		v->Pos = pos;
		return std::move(v);
	}
	static ClassAST* cur_cls = nullptr;
	static std::unique_ptr<ExprAST> ToExpr(unique_ptr<StatementAST>&& v)
	{
		return unique_ptr_cast<ExprAST>(std::move(v));
	}
	typedef  std::unique_ptr<StatementAST>  ptrStatementAST;
	ptrStatementAST  NumberExprAST::Copy()
	{
		return SetPos(make_unique<NumberExprAST>(*this), Pos);
	}

	ptrStatementAST ReturnAST::Copy()
	{
		auto that = make_unique<ReturnAST>(nullptr, Pos);
		that->Val = unique_ptr_cast<ExprAST>(Val->Copy());
		return std::move(that);
	}

	ptrStatementAST  StringLiteralAST::Copy()
	{
		return SetPos(make_unique<StringLiteralAST>(*this),Pos);
	}

	std::unique_ptr<StatementAST> IdentifierExprAST::Copy()
	{
		auto that = make_unique<IdentifierExprAST>(Name);
		that->Pos = Pos;
		if(impl!=nullptr)
			that->impl= unique_ptr_cast<ResolvedIdentifierExprAST>(impl->Copy());
		return std::move(that);
	}

	std::unique_ptr<StatementAST> Birdee::ThisExprAST::Copy()
	{
		return SetPos(make_unique<ThisExprAST>(),Pos);
	}

	std::unique_ptr<StatementAST> Birdee::BoolLiteralExprAST::Copy()
	{
		return SetPos(make_unique<BoolLiteralExprAST>(v),Pos);
	}

	std::unique_ptr<StatementAST> Birdee::NullExprAST::Copy()
	{
		return SetPos(make_unique<NullExprAST>(),Pos);
	}

	ASTBasicBlock Birdee::ASTBasicBlock::Copy()
	{
		ASTBasicBlock ret;
		for (auto& v : body)
		{
			ret.body.push_back(v->Copy());
		}
		return ret;
	}

	std::unique_ptr<StatementAST> Birdee::IfBlockAST::Copy()
	{
		return make_unique<IfBlockAST>(unique_ptr_cast<ExprAST>(cond->Copy()), iftrue.Copy(), iffalse.Copy(), Pos);
	}

	std::unique_ptr<StatementAST> Birdee::ForBlockAST::Copy()
	{
		return make_unique<ForBlockAST>(init->Copy(), unique_ptr_cast<ExprAST>(till->Copy()), including, isdim, block.Copy(), Pos);
	}
	
	std::unique_ptr<StatementAST> Birdee::LoopControlAST::Copy()
	{
		return SetPos(make_unique<LoopControlAST>(tok),Pos);
	}

	std::unique_ptr<StatementAST> Birdee::BinaryExprAST::Copy()
	{
		return make_unique<BinaryExprAST>(Op, unique_ptr_cast<ExprAST>(LHS->Copy()), unique_ptr_cast<ExprAST>(RHS->Copy()), Pos);
	}

	std::unique_ptr<StatementAST> Birdee::IndexExprAST::Copy()
	{
		return make_unique<IndexExprAST>(unique_ptr_cast<ExprAST>(Expr->Copy()), unique_ptr_cast<ExprAST>(Index->Copy()),Pos);
	}

	unique_ptr<Type> Birdee::Type::Copy()
	{
		auto v= make_unique<Type>(this->type);
		v->index_level = index_level;
		return std::move(v);
	}

	unique_ptr<Type> Birdee::IdentifierType::Copy()
	{
		auto v = make_unique<IdentifierType>(name);
		v->index_level = index_level;
		return std::move(v);
	}

	unique_ptr<Type> Birdee::QualifiedIdentifierType::Copy()
	{
		vector<string> copynames = name;
		return make_unique<QualifiedIdentifierType>(std::move(copynames));
	}

	TemplateArgument Birdee::TemplateArgument::Copy()
	{
		return TEMPLATE_ARG_TYPE==kind?TemplateArgument(type->Copy()): TemplateArgument(unique_ptr_cast<ExprAST>(expr->Copy()));
	}

	std::unique_ptr<StatementAST> Birdee::FunctionTemplateInstanceExprAST::Copy()
	{
		vector<TemplateArgument> args;
		for (auto& arg : template_args)
		{
			args.push_back(arg.Copy());
		}
		return make_unique<FunctionTemplateInstanceExprAST>(ToExpr(expr->Copy()),std::move(args),Pos);
	}

	std::unique_ptr<StatementAST> Birdee::AddressOfExprAST::Copy()
	{
		return make_unique<AddressOfExprAST>(ToExpr(expr->Copy()),is_address_of,Pos);
	}

	std::unique_ptr<StatementAST> Birdee::CallExprAST::Copy()
	{
		vector<unique_ptr<ExprAST>> args;
		for (auto& arg : Args)
		{
			args.push_back(ToExpr(arg->Copy()));
		}
		return SetPos(make_unique<CallExprAST>(ToExpr(Callee->Copy()), std::move(args)),Pos);
	}

	std::unique_ptr<StatementAST> Birdee::VariableSingleDefAST::Copy()
	{
		return make_unique<VariableSingleDefAST>(name, type->Copy(), val == nullptr? nullptr:ToExpr(val->Copy()), Pos);
	}

	std::unique_ptr<StatementAST> Birdee::VariableMultiDefAST::Copy()
	{
		vector<unique_ptr<VariableSingleDefAST>> args;
		for (auto& arg : lst)
		{
			args.push_back(unique_ptr_cast<VariableSingleDefAST>(arg->Copy()));
		}
		return make_unique<VariableMultiDefAST>(std::move(args), Pos);
	}

	std::unique_ptr<StatementAST> Birdee::LocalVarExprAST::Copy()
	{
		assert(0 && "LocalVarExprAST cannot be copied");
		return nullptr;
		//return make_unique<LocalVarExprAST>(ToExpr(def->Copy()),Pos);
	}

	std::unique_ptr<PrototypeAST> Birdee::PrototypeAST::Copy()
	{
		return make_unique<PrototypeAST>(Name,unique_ptr_cast<VariableDefAST>(Args->Copy()),RetType->Copy(),cur_cls,pos);
	}

	unique_ptr<TemplateParameters> Birdee::TemplateParameters::Copy()
	{
		vector<Parameter> newparams;
		for (auto& param : params)
		{
			newparams.push_back(std::move(Parameter(param.type == nullptr ? nullptr : param.type->Copy(), param.name)));
		}
		return make_unique<TemplateParameters>(std::move(newparams));
	}

	unique_ptr<StatementAST> Birdee::BasicTypeExprAST::Copy()
	{
		return make_unique<BasicTypeExprAST>(tok,Pos);
	}

	std::unique_ptr<FunctionAST> Birdee::FunctionAST::CopyNoTemplate()
	{
		if (isDeclare)
			throw CompileError(Pos.line, Pos.pos, "Cannot copy a declared function");
		return make_unique<FunctionAST>(Proto->Copy(), Body.Copy(), nullptr, Pos);
	}
	std::unique_ptr<StatementAST> Birdee::FunctionAST::Copy()
	{
		auto cpy = CopyNoTemplate();
		if (template_param)
			cpy->template_param = template_param->Copy();
		return std::move(cpy);
	}
	FieldDef Birdee::FieldDef::Copy()
	{
		return FieldDef(access,unique_ptr_cast<VariableSingleDefAST>(decl->Copy()),index);
	}
	MemberFunctionDef Birdee::MemberFunctionDef::Copy()
	{
		return MemberFunctionDef(access,unique_ptr_cast<FunctionAST>(decl->Copy()));
	}

	std::unique_ptr<StatementAST> Birdee::ResolvedFuncExprAST::Copy()
	{
		assert(0 && "Should not copy ResolvedFuncExprAST");
		return nullptr;
	}

	std::unique_ptr<StatementAST> Birdee::NewExprAST::Copy()
	{
		vector<unique_ptr<ExprAST>> cpyargs;
		for (auto& arg : args)
		{
			cpyargs.push_back(ToExpr(arg->Copy()));
		}
		return make_unique<NewExprAST>(ty->Copy(), std::move(cpyargs),method,Pos);
	}

	std::unique_ptr<StatementAST> Birdee::ClassAST::Copy()
	{
		auto clsdef= make_unique<ClassAST>(name,Pos);
		std::vector<FieldDef>& nfields = clsdef->fields;
		std::vector<MemberFunctionDef>& nfuncs = clsdef->funcs;
		unordered_map<reference_wrapper<const string>, int>& nfieldmap = clsdef->fieldmap;
		unordered_map<reference_wrapper<const string>, int>& nfuncmap = clsdef->funcmap;
		int idx = 0;
		for (auto& v : fields)
		{
			FieldDef def = v.Copy();
			nfieldmap[def.decl->name] = idx;
			nfields.push_back(std::move(def));
			idx++;
		}
		idx = 0;
		for (auto& v : funcs)
		{
			auto def = v.Copy();
			nfuncmap[def.decl->GetName()] = idx;
			nfuncs.push_back(std::move(def));
			idx++;
		}
		return std::move(clsdef);
	}

	std::unique_ptr<StatementAST> Birdee::MemberExprAST::Copy()
	{
		return make_unique<MemberExprAST>(ToExpr(Obj->Copy()),member);
	}

}