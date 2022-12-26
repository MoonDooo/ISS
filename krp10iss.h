#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/*
* �Լ� �� ����: krp10iss.h
*/
#define SIZE 1024 // �޸� 4KB = WORD * 1024
#define WORD_SIZE 4 // WORD = 4BYTE
#define REG_NUM 32 
#define IMM17_BITS 17 
#define IMM22_BITS 22 
#define REG_POINTER_BITS 5 // ���������� �ּҰ��� ������ ��Ʈ ��
#define SHAMT_BIT 6
#define I_BIT 1 
#define COND_BIT 3 
#define Total_Bits 32 
//�й�
#define STUDENT_NUMBER 20181405;

unsigned char* PMemory[WORD_SIZE];
unsigned char* DMemory[WORD_SIZE];
bool isAccessMemory[SIZE];
unsigned int R[REG_NUM];
unsigned int IR;
unsigned int PC;
//�̵� ������ ���� �߰��� ��

//OPERAND ��Ʈ �ʵ� ����ü
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

//cmdâ���� input���ϸ�� output���ϸ� �Է��� ����� ���� ����Ͽ� ���� �Լ� default�� input_file�� output_file�� �����Ѵ�.
void InitFileName(int argc, char* argv[]);
//���۽� ȭ�� ���� �Լ�
void PrintStart();
//��ɾ� �޸𸮿� ������ �޸𸮸� �ʱ�ȭ�ϴ� �Լ� PC���� �̶� �ʱ�ȭ �ȴ�.
void InitMemory();
//�Էµ� argv[1] (default�� input_file)�� �Էµ� ��η� ������ �д� �Լ�.
void InputData(char* fileName);
//����ڷκ��� ��ɾ �Է¹޴´�. ( r , c , h )
void RequestCommand(char* outputFileName);
//���������� �߰��� �Լ��μ� h �Է¹����� ��ɾ���� ������ ��µǴ� �Լ�
void PrintHelp();
//RequestCommand �Լ��κ��� �Է¹��� ��ɾ �����Ű�� �Լ�
void CommandProcessing(char command, char* outputFileName);
//sĿ�ǵ带 �Է¹����� �����ϴ� �Լ��̴�.
void ExecuteNextInst();
//rĿ�ǵ带 �Է¹����� �����ϴ� �Լ��̴�.
void AllInstExecute();
//��ɾ �����ϴ� �Լ��̴�.
void InstructionExecute();
//REGISTER ����� ��� �Լ��̴�.
void PrintReg();

//PRINT REGISTER �Լ�
void printSReg();
void printGReg();
//�Ϲ� �������͸� 16������ ����ϴ� �ӽ� �Լ�, ����� �ּ�ó��
void printGReg2();
//�䱸���׿� ���� ������ �޸� �� ���
void printAccessMemory();

//InstructionExecute �Լ�
void Fetch();
//DECODE�� INSTRUCTION�� �ٷ� EXECUTE�Լ��� �Ѿ��.
void Decode();

//OPERAND READ �Լ�
int OPCODE();
void RARead(Operand* operand);
void RBRead(Operand* operand);
void RCRead(Operand* operand);
void IRead(Operand* operand);
void ShamtRead(Operand* operand);
void CondRead(Operand* operand);
void Imm22Read(Operand* operand);
void Imm17Read(Operand* operand);

//EXECUTE �Լ�
//��ȣȮ��
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


//WRITE �Լ�
void RegisterStr(unsigned reg, unsigned int result);


//MEMORY ACCESS �Լ�
unsigned ReadMemory(unsigned idx, unsigned char* memory[]);
void WriteMemory(unsigned idx, unsigned data, unsigned char* memory[]);

