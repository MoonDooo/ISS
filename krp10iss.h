#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/*
* 함수 각 설명: krp10iss.h
*/
#define SIZE 1024 // 메모리 4KB = WORD * 1024
#define WORD_SIZE 4 // WORD = 4BYTE
#define REG_NUM 32 
#define IMM17_BITS 17 
#define IMM22_BITS 22 
#define REG_POINTER_BITS 5 // 레지스터의 주소값을 가지는 비트 수
#define SHAMT_BIT 6
#define I_BIT 1 
#define COND_BIT 3 
#define Total_Bits 32 
//학번
#define STUDENT_NUMBER 20181405;

unsigned char* PMemory[WORD_SIZE];
unsigned char* DMemory[WORD_SIZE];
bool isAccessMemory[SIZE];
unsigned int R[REG_NUM];
unsigned int IR;
unsigned int PC;
//이따 순서나 설명 추가할 것

//OPERAND 비트 필드 구현체
typedef struct {
	unsigned ra : REG_POINTER_BITS;
	unsigned rb : REG_POINTER_BITS;
	unsigned rc : REG_POINTER_BITS;
	unsigned shamt : SHAMT_BIT;
	unsigned I : I_BIT; 
	unsigned cond : COND_BIT;
	signed imm22 : IMM22_BITS;
	signed imm17 : IMM17_BITS;
}Operand;

//cmd창으로 input파일명과 output파일명 입력이 어려울 것을 대비하여 만든 함수 default로 input_file과 output_file을 저장한다.
void InitFileName(int argc, char* argv[]);
//시작시 화면 구성 함수
void PrintStart();
//명령어 메모리와 데이터 메모리를 초기화하는 함수 PC값도 이때 초기화 된다.
void InitMemory();
//입력된 argv[1] (default는 input_file)에 입력된 경로로 파일을 읽는 함수.
void InputData(char* fileName);
//사용자로부터 명령어를 입력받는다. ( r , c , h )
void RequestCommand(char* outputFileName);
//임의적으로 추가한 함수로서 h 입력받을시 명령어관련 설명이 출력되는 함수
void PrintHelp();
//RequestCommand 함수로부터 입력받은 명령어를 실행시키는 함수
void CommandProcessing(char command, char* outputFileName);
//s커맨드를 입력받을시 수행하는 함수이다.
void ExecuteNextInst();
//r커맨드를 입력받을시 수행하는 함수이다.
void AllInstExecute();
//명령어를 수행하는 함수이다.
void InstructionExecute();
//REGISTER 결과값 출력 함수이다.
void PrintReg();

//PRINT REGISTER 함수
void printSReg();
void printGReg();
//일반 레지스터를 16진수로 출력하는 임시 함수, 제출시 주석처리
void printGReg2();
//요구사항에 따라 접근한 메모리 값 출력
void printAccessMemory();

//InstructionExecute 함수
void Fetch();
//DECODE한 INSTRUCTION은 바로 EXECUTE함수로 넘어간다.
void Decode();

//OPERAND READ 함수
int OPCODE();
void RARead(Operand* operand);
void RBRead(Operand* operand);
void RCRead(Operand* operand);
void IRead(Operand* operand);
void ShamtRead(Operand* operand);
void CondRead(Operand* operand);
void Imm22Read(Operand* operand);
void Imm17Read(Operand* operand);

//EXECUTE 함수
//부호확장
unsigned signExt(unsigned input, int bits);

void ADDI();
void ORI();
void ANDI();
void MOVI();
void ADD();
void SUB();
void NOT();
void NEG();
void OR();
void AND();
void XOR();
void ASR();
void LSR();
void SHL();
void ROR();
void BR();
void BRL();
void J();
void JL();
void LD();
void LDR();
void ST();
void STR();
void LEA();
void MSN();


//WRITE 함수
void RegisterStr(unsigned reg, unsigned int result);


//MEMORY ACCESS 함수
unsigned ReadMemory(unsigned idx, unsigned char* memory[]);
void WriteMemory(unsigned idx, unsigned data, unsigned char* memory[]);

