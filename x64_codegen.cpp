#include <ostream>
#include "3ac.hpp"

namespace drewgon{

void IRProgram::allocGlobals(){
	//Choose a label for each global
	//Choose a label for each global
	for (auto g: globals)
	{
		SymOpd * globalOpd = g.second;
		std::string memLoc = "glb_";
		const SemSymbol * sym = globalOpd->getSym();
		memLoc += sym->getName();
		globalOpd->setMemoryLoc("(" + memLoc + ")");
	}
}

void IRProgram::datagenX64(std::ostream& out){
	for (auto cur : strings)
	{
		out << "str_text:";
		out << "\t.asciz \"" + cur.second + "\";\n";
	}
	out << ".globl main\n";
	for (auto g: globals)
	{
		SymOpd * globalOpd = g.second;
		std::string memLoc = "glb_";
		const SemSymbol * sym = globalOpd->getSym();
		memLoc += sym->getName();
		size_t width = sym->getDataType()->getSize();
		out << memLoc << ": ";
		if (width == 8)
		{
			out << ".quad 0 \n";
		}
		else
		{
			out << ".space " << width << "\n";
		}
		
	}
	//Put this directive after you write out strings
	// so that everything is aligned to a quadword value
	// again
	out << ".align 8\n";

}

void IRProgram::toX64(std::ostream& out){
	allocGlobals();
	datagenX64(out);
	// Iterate over each procedure and codegen it
	for (auto proc: *this->procs)
	{
		proc->toX64(out);
	}
}

void Procedure::allocLocals(){
	//Allocate space for locals
	// Iterate over each procedure and codegen it
	int size = -16;
	for (auto t : temps)
	{
		t->setMemoryLoc(to_string(size));
		size = size - 8;
	}

	for (auto l : locals)
	{
		l.second->setMemoryLoc(to_string(size));
		size = size - 8;
	}

	for (auto f : formals)
	{
		f->setMemoryLoc(to_string(size));
		size = size - 8;
	}

	for (auto t : addrOpds)
	{
		TODO(Implement me)
	}
}

void Procedure::toX64(std::ostream& out){
	//Allocate all locals
	allocLocals();

	enter->codegenLabels(out);
	enter->codegenX64(out);
	out << "#Fn body " << myName << "\n";
	for (auto quad : *bodyQuads){
		quad->codegenLabels(out);
		out << "#" << quad->toString() << "\n";
		quad->codegenX64(out);
	}
	out << "#Fn epilogue " << myName << "\n";
	leave->codegenLabels(out);
	leave->codegenX64(out);
}

void Quad::codegenLabels(std::ostream& out){
	if (labels.empty()){ return; }

	size_t numLabels = labels.size();
	size_t labelIdx = 0;
	for ( Label * label : labels){
		out << label->getName() << ": ";
		if (labelIdx != numLabels - 1){ out << "\n"; }
		labelIdx++;
	}
}

void BinOpQuad::codegenX64(std::ostream& out){
	
	BinOp op = this->getOp();
	if(op == ADD64)
	{
		src1->genLoadVal(out, A);
		src2->genLoadVal(out, B);
		out << "addq " << "%rax, " << "%rbx\n" ;
		dst->genStoreVal(out, B);

	}

}

void UnaryOpQuad::codegenX64(std::ostream& out){
	src->genLoadVal(out, A);
	dst->genStoreVal(out, A);
}

void AssignQuad::codegenX64(std::ostream& out){
	src->genLoadVal(out, A);
	dst->genStoreVal(out, A);
}

void GotoQuad::codegenX64(std::ostream& out){
	out << "jmp " << tgt->getName() << "\n";
}

void IfzQuad::codegenX64(std::ostream& out){
	cnd->genLoadVal(out, A);
	std::cout << cnd->locString();

	out << "cmpq $0, %rax\n";
	out << "je lbl_" << tgt->getName() << "\n";
}

void NopQuad::codegenX64(std::ostream& out){
	out << "nop" << "\n";
}

void IntrinsicMayhemQuad::codegenX64(std::ostream& out){
	TODO(Implement me)
}

void IntrinsicOutputQuad::codegenX64(std::ostream& out){
	myArg->genLoadVal(out, DI);
	if (myArg->getWidth() == 8)
		out << "callq printInt\n";
	else if (myArg->getWidth() == 1)
		out << "callq printByte\n";
	else
		out << "callq printString";
}

void IntrinsicInputQuad::codegenX64(std::ostream& out){
	myArg->genStoreVal(out, A);
	if (myArg->getWidth() == 8)
	out << "callq writeInt\n";
	else if (myArg->getWidth() == 1)
	out << "callq writeByte\n";
	else
	out << "callq writeString";
}

void CallQuad::codegenX64(std::ostream& out){
	out << "call " << callee->getName() << "\n";
}

void EnterQuad::codegenX64(std::ostream& out){
	out << "pushq %rbp" << "\n";
	out << "movq %rsp, %rbp" << "\n";
	out << "addq $16, %rbp" << "\n";
	out << "subq $" << myProc->arSize() << ", %rsp" << "\n";
}

void LeaveQuad::codegenX64(std::ostream& out){
	out << "addq $" << myProc->arSize() << ", %rsp" << "\n";
	out << "popq %rbp" << "\n";
	out << "retq" << "\n";
}

void SetArgQuad::codegenX64(std::ostream& out){
		if (index == 1)
			opd->genLoadVal(out, DI);
		else if (index == 2)
			opd->genLoadVal(out, A);
		else if (index == 3)
			opd->genLoadVal(out, D);
		else if (index == 4)
			opd->genLoadVal(out, C);
		else if (index == 5)
			opd->genLoadVal(out, E);
		else if (index == 6)
			opd->genLoadVal(out, F);
		else
		{
			opd->genLoadVal(out, A);
			out << "pushq %rax\n";
		}
}

void GetArgQuad::codegenX64(std::ostream& out){
	//We don't actually need to do anything here
}

void SetRetQuad::codegenX64(std::ostream& out){
	opd->genLoadVal(out, A);
}

void GetRetQuad::codegenX64(std::ostream& out){
	opd->genStoreVal(out, A);
}

void LocQuad::codegenX64(std::ostream& out){
	TODO(Implement me)
}

void SymOpd::genLoadVal(std::ostream& out, Register reg){
	std::string output = "";
		if (this->getWidth() == 1)
		{
			if (this->locString() != "true" && this->locString() != "false")
			{
				char var = this->valString()[1];
				int ascii = int(var);
				output = to_string(ascii);
			}
		}

		out << "movq " << myLoc << "(%rbp), " << getReg(reg) << "\n";
		output = myLoc;
}

void SymOpd::genStoreVal(std::ostream& out, Register reg){
	std::string loc = this->getMemoryLoc();
	out << getMovOp() << getReg(reg) << ", " << loc << "(%rbp)\n";
}

void SymOpd::genLoadAddr(std::ostream& out, Register reg) {
	out << getMovOp() << getMemoryLoc() << '\n';
}

void AuxOpd::genLoadVal(std::ostream& out, Register reg){
	std::string output = "";

		if (this->getWidth() == 1)
		{
			if (this->valString() != "true" && this->valString() != "false")
			{
				char var = valString()[1];
				int ascii = int(var);
				output = to_string(ascii);
				out << this->getMovOp() << output << ", " << getReg(reg) << "\n";
			}
			else
				out << this->getMovOp() << myLoc << "(%rbp), " << getReg(reg) << "\n";
		}
		else
			out << this->getMovOp() << myLoc << "(%rbp), " << getReg(reg) << "\n";
}

void AuxOpd::genStoreVal(std::ostream& out, Register reg){
	out << "movq " << getReg(reg) << ", " << myLoc << "(%rbp)" << "\n";
}
void AuxOpd::genLoadAddr(std::ostream& out, Register reg){
	TODO(Implement me)
}

void AddrOpd::genStoreVal(std::ostream& out, Register reg){
	TODO(Implement me)
}

void AddrOpd::genLoadVal(std::ostream& out, Register reg){
	TODO(Implement me)
}

void AddrOpd::genStoreAddr(std::ostream& out, Register reg){
	TODO(Implement me)
}

void AddrOpd::genLoadAddr(std::ostream & out, Register reg){
	TODO(Implement me)
}

void LitOpd::genLoadVal(std::ostream & out, Register reg){
	out << getMovOp() << " $" << val << ", " << getReg(reg) << "\n";
}

}
