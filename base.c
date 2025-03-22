
/*  call_stack
    
    실제 시스템에서는 스택이 메모리에 저장되지만, 본 과제에서는 `int` 배열을 이용하여 메모리를 구현합니다.
    원래는 SFP와 Return Address에 실제 가상 메모리 주소가 들어가겠지만, 이번 과제에서는 -1로 대체합니다.
    
    int call_stack[]      : 실제 데이터(`int 값`) 또는 `-1` (메타데이터 구분용)을 저장하는 int 배열 
    char stack_info[][]   : call_stack[]과 같은 위치(index)에 대한 설명을 저장하는 문자열 배열

    ==========================call_stack 저장 규칙==========================
    매개 변수 / 지역 변수를 push할 경우   : int 값 그대로
    Saved Frame Pointer 를 push할 경우  : call_stack에서의 index
    반환 주소값을 push할 경우       : -1
    =======================================================================
    

    ==========================stack_info 저장 규칙==========================
    매개 변수 / 지역 변수를 push할 경우        : 변수에 대한 설명
    Saved Frame Pointer 를 push할 경우  : 어떤 함수의 SFP인지
    반환 주소값을 push할 경우                 : "Return Address"
    ========================================================================
*/
#include <stdio.h>
#include <string.h>
#define STACK_SIZE 50 // 최대 스택 크기

int     call_stack[STACK_SIZE];         // Call Stack을 저장하는 배열
char    stack_info[STACK_SIZE][20];     // Call Stack 요소에 대한 설명을 저장하는 배열

/*  SP (Stack Pointer), FP (Frame Pointer)

    SP는 현재 스택의 최상단 인덱스를 가리킵니다.
    스택이 비어있을 때 SP = -1, 하나가 쌓이면 `call_stack[0]` -> SP = 0, `call_stack[1]` -> SP = 1, ...

    FP는 현재 함수의 스택 프레임 포인터입니다.
    실행 중인 함수 스택 프레임의 sfp를 가리킵니다.
*/
int SP = -1; 
int FP = -1;

void func1(int arg1, int arg2, int arg3);
void func2(int arg1, int arg2);
void func3(int arg1);

/*  
    현재 call_stack 전체를 출력합니다.
    해당 함수의 출력 결과들을 바탕으로 구현 완성도를 평가할 예정입니다.
*/
void print_stack()
{
    if (SP == -1)
    {
        printf("Stack is empty.\n");
        return;
    }

    printf("====== Current Call Stack ======\n");
    
    for (int i = SP; i >= 0; i--)
    {
        if (call_stack[i] != -1)
            printf("%d : %s = %d", i ,stack_info[i], call_stack[i]);
        else
            printf("%d : %s", i, stack_info[i]);

        if (i == SP)
            printf("    <=== [esp]\n");
        else if (i == FP)
            printf("    <=== [ebp]\n");
        else
            printf("\n");
    }
    printf("================================\n\n");
}


//func 내부는 자유롭게 추가해도 괜찮으나, 아래의 구조를 바꾸지는 마세요
void func1(int arg1, int arg2, int arg3)
{
    SP += 3;
    call_stack[0] = arg3;
    strcpy(stack_info[0], "arg3");
    call_stack[1] = arg2;
    strcpy(stack_info[1], "arg2");
    call_stack[2] = arg1;
    strcpy(stack_info[2], "arg1");

    int var_1 = 100;

    // func1의 스택 프레임 형성 (함수 프롤로그 + push)
    SP += 2;
    call_stack[3] = -1;
    strcpy(stack_info[3], "Return Address");
    call_stack[4] = -1; // main stack frame 구현 안되어있음.
    strcpy(stack_info[4], "func1 SFP");
    FP += 5;
    SP ++;
    call_stack[5] = 100;
    strcpy(stack_info[5] , "var_1");
    print_stack();

    func2(11, 13);
    // func2의 스택 프레임 제거 (함수 에필로그 + pop)
    SP --; // 지역변수
    FP = 4;
    SP -= 4;

    print_stack();
}


void func2(int arg1, int arg2)
{
    SP += 2;
    call_stack[6] = 13;
    strcpy(stack_info[6], "arg2");
    call_stack[7] = 11;
    strcpy(stack_info[7], "arg1");

    int var_2 = 200;

    // func2의 스택 프레임 형성 (함수 프롤로그 + push)
    SP += 2;
    call_stack[8] = -1;
    strcpy(stack_info[8], "Return Address");
    call_stack[9] = 4;
    strcpy(stack_info[9], "func2 SFP");
    FP = 9;
    SP ++;
    call_stack[10] = 200;
    strcpy(stack_info[10], "var_2");

    print_stack();

    func3(77);
    // func3의 스택 프레임 제거 (함수 에필로그 + pop)

    SP -= 2;
    FP = 9;
    SP -= 3;



    print_stack();
}


void func3(int arg1)
{
    SP ++;
    call_stack[11] = 77;
    strcpy(stack_info[11], "arg1");

    int var_3 = 300;
    int var_4 = 400;

    // func3의 스택 프레임 형성 (함수 프롤로그 + push)
    SP +=2;
    call_stack[12] = -1;
    strcpy(stack_info[12], "Return Address");
    call_stack[13] = 9;
    strcpy(stack_info[13], "func3 SFP");
    FP = 13;

    SP += 2;
    call_stack[14] = 300;
    strcpy(stack_info[14], "var_3");
    call_stack[15] = 400;
    strcpy(stack_info[15], "var_4");

    print_stack();
}


//main 함수에 관련된 stack frame은 구현하지 않아도 됩니다.
int main()
{
    func1(1, 2, 3);
    // func1의 스택 프레임 제거 (함수 에필로그 + pop)
    SP -= 6;

    print_stack();
    return 0;
}