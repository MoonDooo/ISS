#include "krp10iss.h"

int main(int argc, char* argv[]) {
	InitFileName(argc, argv);
	PrintStart();
	InitMemory();
	InputData(argv[1]);
	while (1) {
		RequestCommand(argv[2]);
	}
}
void InitFileName(int argc, char* argv[]) {
	if (argc == 1) {
		argv[1] = "input_file";
		argv[2] = "output_file";
	}
	if (argc == 2) {
		argv[2] = "output_file";
	}
}
void PrintStart() {
	printf("==================================================================================================================\n");
	printf("\t\t\t\t  KRP IX Instruction Set Simulator\n");
	printf("==================================================================================================================\n");
	printf("Request (help : h):\n");
}
void InitMemory() {
	for (int i = 0; i < WORD_SIZE; i++) {
		PMemory[i] = (unsigned char*)malloc(SIZE * sizeof(unsigned char));
		DMemory[i] = (unsigned char*)malloc(SIZE * sizeof(unsigned char));
		memset(PMemory[i], 0, SIZE*sizeof(unsigned char));
		memset(DMemory[i], 0, SIZE*sizeof(unsigned char));
	}
	for (int i = 0; i < SIZE; i++) {
		isAccessMemory[i] = false;
	}
	PC = 0;
}
void InputData(char* fileName) {
	FILE* inputFile;

	if (fileName == NULL) {
		fileName = "input_file";
	}
	inputFile = fopen(fileName, "r+b");
	if (inputFile == NULL) { printf("파일을 찾지 못했습니다."); exit(1); } 
	
	for (int i = 0; i < SIZE; i++) {
		if (feof(inputFile))break;
		for (int j = 0; j < WORD_SIZE; j++) {
			fread(&PMemory[j][i], sizeof(unsigned char), 1, inputFile);
			printf("%08X  ", PMemory[j][i]);
		}	
		printf("\n");
	}		

	fclose(inputFile);
}
void outputData(char* fileName) {
	FILE* outputFile;

	outputFile = fopen(fileName, "w+t");
	if (feof(outputFile)) {	printf("Cannot open the file.\n"); exit(1);}

	for(int i = 0; i<SIZE; i++){
		fprintf(outputFile, "0x%08X : ", i);
		for (int j = 0; j < WORD_SIZE; j++) {
			fprintf(outputFile, "%02X ", DMemory[j][i]);
		}
		fprintf(outputFile, "\n");
	}


	fclose(outputFile);
}

void RequestCommand(char* outputFileName) {
	printf(">>");
	CommandProcessing(getchar(), outputFileName);
	getchar(); // 입력버퍼 비우기
}
void CommandProcessing(char command, char* outputFileName) {
	switch (command) {
	case 's':
		ExecuteNextInst();
		break;
	case 'r':
		AllInstExecute();
		outputData(outputFileName);
		printAccessMemory();
		break;
	case 'q':
		exit(0);
		break;
	case 'h':
		PrintHelp();
		break;
	}
	
}
void printSReg(){
	printf("PC:   %d\n", PC);
	printf("Inst: %08X", IR);
}
void printGReg() {
	for (int i = 0; i < REG_NUM; i++) {
		if (i % WORD_SIZE == 0) {
			printf("\n");
		}
		printf("R[%2d] : %d \t", i, (signed)R[i]);
	}
	printf("\n");
}

void printGReg2() {
	for (int i = 0; i < REG_NUM; i++) {
		if (i % WORD_SIZE == 0) {
			printf("\n");
		}
		printf("R[%2d] : %08X \t", i, (signed)R[i]);
	}
	printf("\n");
}
void PrintHelp() {
	printf("s : 다음 명령어 수행\nr : 모든 명령어 수행\nh : help\nq : 나가기\n");
}

void ExecuteNextInst() {
	InstructionExecute();
	PrintReg();
}
void AllInstExecute() {
	while (PC/4<SIZE) {
		InstructionExecute();
	}
	PrintReg();
}

void PrintReg() {
	printSReg();
	printGReg();
	//printGReg2();
}

void InstructionExecute() {
	Fetch();
	Decode();
}
/*
* 접근하였던 위치의 데이터 메모리 내용을 표시
*/
void printAccessMemory() {
	for (int i = 0; i < SIZE; i++) {
		if (isAccessMemory[i]) {
			printf("0x%08X : ", i);
			for (int j = 0; j < WORD_SIZE; j++) {
				printf("%02X ", DMemory[j][i]);
			}
			printf("\n");
		}
	}
}

/*
* Fetch 함수
*/
void Fetch() {
	IR = ReadMemory(PC / WORD_SIZE, PMemory);
	PC += 4;
}
/*
* Decode 함수
*/
void Decode() {
	void (*ExecuteOPCODE[32])() = {
	ADDI,	NULL,	ORI,	NULL,	ANDI,	NULL,
	MOVI,	NULL,	ADD,	SUB,	NOT,	NEG,
	OR,		AND,	XOR,	ASR,	LSR,	SHL,
	ROR,	BR,		BRL,	J,		JL,		LD,
	LDR,	ST,		STR,	LEA,	MSN,	NULL,
	NULL,	NULL,
	};
	ExecuteOPCODE[OPCODE()]();
}
/*
* Memory Access 함수
*/
unsigned ReadMemory(unsigned idx, unsigned char* memory[]) {
	unsigned out = 0;
	for (int i = 0; i < WORD_SIZE; i++) {
		out |= memory[i][idx] << (8 * i);
	}
	return out;
}

/*
* write 함수
*/
void WriteMemory(unsigned idx, unsigned data, unsigned char* memory[]) {
	isAccessMemory[idx] = true;
	for (int i = 0; i < WORD_SIZE; i++) {
		memory[i][idx] = data >> (8 * i);
	}
}
void RegisterStr(unsigned reg, unsigned int result) {
	R[reg] = result;
}

/*
* Read Operand 함수
*/
int OPCODE() {return (IR >> 27);}
void RARead	  (Operand* operand){	 operand->ra	= IR >> 22;}
void RBRead	  (Operand* operand){	 operand->rb	= IR >> 17;}
void RCRead	  (Operand* operand){	 operand->rc	= IR >> 12;}
void IRead	  (Operand* operand){	 operand->I		= IR >> 5;}
void ShamtRead(Operand* operand){	 operand->shamt = IR; }
void CondRead (Operand* operand){	 operand->cond	= IR;}
void Imm22Read(Operand* operand){	 operand->imm22	= IR;}
void Imm17Read(Operand* operand){	 operand->imm17 = IR;}

/*
* Execute 함수
*/
unsigned signExt(unsigned input, int bits) {
	return (input >> (bits - 1)) ? ((0xFFFFFFFF << bits) | input) : input;
}


void ADDI() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RBRead(&operand);
	Imm17Read(&operand);
	//Execute
	unsigned result = R[operand.rb] + signExt(operand.imm17, IMM17_BITS);
	//Write
	RegisterStr(operand.ra, result);
}
void ORI() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RBRead(&operand);
	Imm17Read(&operand);
	//Execute
	unsigned result = R[operand.rb] | signExt(operand.imm17, IMM17_BITS);
	//write
	RegisterStr(operand.ra, result);
}
void ANDI() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RBRead(&operand);	
	Imm17Read(&operand);
	//Execute
	unsigned result = R[operand.rb] & signExt(operand.imm17, IMM17_BITS);
	//write
	RegisterStr(operand.ra, result);
}
void MOVI() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	Imm17Read(&operand);
	//Execute
	unsigned result = signExt(operand.imm17, IMM17_BITS);
	//write
	RegisterStr(operand.ra, result);
}
void ADD() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RBRead(&operand);
	RCRead(&operand);
	//Execute
	unsigned result = R[operand.rb] + R[operand.rc];
	//write
	RegisterStr(operand.ra, result);
}
void SUB() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RCRead(&operand);
	RBRead(&operand);
	//Execute
	unsigned result = R[operand.rb] - R[operand.rc];
	//write
	RegisterStr(operand.ra, result);
}
void NOT() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RCRead(&operand);
	//Execute
	unsigned result = ~R[operand.rc];
	//write
	RegisterStr(operand.ra, result);
}
void NEG() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RCRead(&operand);
	//Execute
	unsigned result = -(signed)R[operand.rc];
	//write
	RegisterStr(operand.ra, result);
}
void OR() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RBRead(&operand);
	RCRead(&operand);	
	//Execute
	unsigned result = R[operand.rb] | R[operand.rc];
	//write
	RegisterStr(operand.ra, result);
}
void AND() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RBRead(&operand);
	RCRead(&operand);
	//Execute
	unsigned result = R[operand.rb] & R[operand.rc];
	//write
	RegisterStr(operand.ra, result);
}
void XOR() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RBRead(&operand);
	RCRead(&operand);
	//Execute
	unsigned result = R[operand.rb] ^ R[operand.rc];
	//write
	RegisterStr(operand.ra, result);
}
void ASR() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RBRead(&operand);
	RCRead(&operand);
	IRead(&operand);
	ShamtRead(&operand);	
	//Execute
	unsigned result;
	if (operand.I == 0) {
		result = signExt((R[operand.rb] >> operand.shamt), Total_Bits - operand.shamt);
	}
	else {
		result = signExt(R[operand.rb] >> R[operand.rc], Total_Bits - R[operand.rc]);
	} 
	//write
	RegisterStr(operand.ra, result);
}
void LSR() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RBRead(&operand);
	RCRead(&operand);
	IRead(&operand);
	ShamtRead(&operand);
	//Execute
	unsigned result;
	if (operand.I == 0) {
		result = R[operand.rb] >> operand.shamt;
	}
	else {
		result = R[operand.rb] >> R[operand.rc];
	}
	//write
	RegisterStr(operand.ra, result);
}
void SHL() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RBRead(&operand);
	RCRead(&operand);
	IRead(&operand);
	ShamtRead(&operand);
	//Execute
	unsigned result;
	if (operand.I == 0) {
		result = R[operand.rb] << operand.shamt;
	}
	else {
		result = R[operand.rb] << R[operand.rc];
	}
	//write
	RegisterStr(operand.ra, result);
}
void ROR() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RBRead(&operand);
	RCRead(&operand);
	IRead(&operand);
	ShamtRead(&operand);
	//Execute
	unsigned result;
	if (operand.I == 0) {
		result = R[operand.rb] >> operand.shamt | R[operand.rb] << (Total_Bits-operand.shamt);
	}
	else {
		result = R[operand.rb] >> R[operand.rc] | R[operand.rb] << (Total_Bits-R[operand.rc]);
	}
	//write
	RegisterStr(operand.ra, result);
}
void BR() {
	Operand operand;
	//ReadOperand
	RBRead(&operand);
	RCRead(&operand);
	CondRead(&operand);
	//Execute
	switch(operand.cond) {
		case 0:		/* Never */
			break;
		case 1:		/* Always */
			PC = R[operand.rb];
			break;
		case 2:		/* Zero */
			if (R[operand.rc] == 0)
				PC = R[operand.rb];
			break;
		case 3:		/* Nonzero */
			if (R[operand.rc] != 0)
				PC = R[operand.rb];
			break;
		case 4:		/* Plus */
			if (R[operand.rc] >= 0)
				PC = R[operand.rb];
			break;	
		case 5:		/* Minus */
			if (R[operand.rc] < 0)
				PC = R[operand.rb];
			break;
	}
	//write
	//nothing
}
void BRL() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RBRead(&operand);
	RCRead(&operand);
	CondRead(&operand);
	//Execute
	unsigned currentPC = PC;
	switch (operand.cond) {
	case 0:		/* Never */
		break;
	case 1:		/* Always */
		PC = R[operand.rb];
		break;
	case 2:		/* Zero */
		if (R[operand.rc] == 0)
			PC = R[operand.rb];
		break;
	case 3:		/* Nonzero */
		if (R[operand.rc] != 0)
			PC = R[operand.rb];
		break;
	case 4:		/* Plus */
		if (R[operand.rc] >= 0)
			PC = R[operand.rb];
		break;
	case 5:		/* Minus */
		if (R[operand.rc] < 0)
			PC = R[operand.rb];
		break;
	}
	//write
	RegisterStr(operand.ra, currentPC);
}
void J() {
	Operand operand;
	//ReadOperand
	Imm22Read(&operand);
	//Execute
	PC += signExt(operand.imm22, IMM22_BITS);
	//write
}
void JL() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	Imm22Read(&operand);
	//Execute
	unsigned currentPC = PC;
	PC += signExt(operand.imm22, IMM22_BITS);
	//write
	RegisterStr(operand.ra, currentPC);
}
void LD() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RBRead(&operand);
	Imm17Read(&operand);
	//Execute
	unsigned result;
	if (operand.rb == 0b11111)
		result = ReadMemory(operand.imm17, DMemory);
	else
		result = ReadMemory(signExt(operand.imm17,IMM17_BITS) + R[operand.rb], DMemory);
	//write
	R[operand.ra] = result;
}
void LDR() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	Imm22Read(&operand);
	//Execute
	unsigned result;
	result = ReadMemory(PC + signExt(operand.imm22, IMM22_BITS), DMemory);
	//write
	R[operand.ra] = result;
}
void ST() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	RBRead(&operand);
	Imm17Read(&operand);
	//Execute
	if (operand.rb == 0b11111) {
		WriteMemory(operand.imm17, R[operand.ra], DMemory);
	}
	else {
		WriteMemory(R[operand.rb] + signExt(operand.imm17, IMM17_BITS), R[operand.ra], DMemory);
	}
	//write
}
void STR() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	Imm22Read(&operand);
	//Execute
	WriteMemory(PC + signExt(operand.imm22, IMM22_BITS), R[operand.ra], DMemory);
	//write
}
void LEA() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	Imm22Read(&operand);
	//Execute
	unsigned result;
	result = PC + signExt(operand.imm22, IMM22_BITS);
	//write
	R[operand.ra] = result;
}
void MSN() {
	Operand operand;
	//ReadOperand
	RARead(&operand);
	//Execute
	unsigned result = STUDENT_NUMBER;
	//write
	R[operand.ra] = result;
}

