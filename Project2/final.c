#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student {
    char name[20];
    int kor, eng, math;
    int total;
    int avg;
    int rank;
    struct Student* next;
} Student;

Student* head = NULL;

/* ================= 파일에서 데이터 읽기 ================= */
void loadFromFile() {
    FILE* fp = fopen("student.dat", "rb");
    if (fp == NULL) {
        printf("저장된 파일이 없습니다.\n\n");
        return;
    }

    // 기존 리스트 메모리 해제
    Student* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }

    Student data;
    while (fread(&data, sizeof(Student), 1, fp) == 1) {
        Student* newNode = (Student*)malloc(sizeof(Student));
        *newNode = data;
        newNode->next = NULL;

        if (head == NULL)
            head = newNode;
        else {
            temp = head;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = newNode;
        }
    }

    fclose(fp);
    printf("파일에서 데이터 읽기 완료\n\n");
}

/* ================= 파일에 데이터 저장 ================= */
void saveToFile() {
    FILE* fp = fopen("student.dat", "wb");
    if (fp == NULL) {
        printf("파일 저장 실패\n\n");
        return;
    }

    Student* temp = head;
    while (temp != NULL) {
        fwrite(temp, sizeof(Student), 1, fp);
        temp = temp->next;
    }

    fclose(fp);
    printf("파일 저장 완료\n\n");
}

/* ================= 학생 정보 입력 ================= */
void inputStudent() {
    Student* newNode = (Student*)malloc(sizeof(Student));

    printf("이름: ");
    scanf_s("%s", newNode->name, (unsigned)_countof(newNode->name));

    printf("국어 점수: ");
    scanf_s("%d", &newNode->kor);

    printf("영어 점수: ");
    scanf_s("%d", &newNode->eng);

    printf("수학 점수: ");
    scanf_s("%d", &newNode->math);

    newNode->total = newNode->kor + newNode->eng + newNode->math;
    newNode->avg = newNode->total / 3;
    newNode->rank = 1;
    newNode->next = NULL;

    if (head == NULL)
        head = newNode;
    else {
        Student* temp = head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }

    printf("학생 정보 입력 완료\n\n");
}

/* ================= 등수 계산 ================= */
void calculateRank() {
    Student* p;
    Student* q;

    for (p = head; p != NULL; p = p->next) {
        p->rank = 1;
        for (q = head; q != NULL; q = q->next) {
            if (p->total < q->total)
                p->rank++;
        }
    }
}

/* ================= 성적 출력 ================= */
void printStudents() {
    if (head == NULL) {
        printf("출력할 데이터가 없습니다.\n\n");
        return;
    }

    calculateRank();

    printf("-------------------------------------------------\n");
    printf("이름   국어  영어  수학  총점  평균  등수\n");
    printf("-------------------------------------------------\n");

    Student* temp = head;
    while (temp != NULL) {
        printf("%-6s %4d %4d %4d %4d %4d %4d\n",
            temp->name,
            temp->kor,
            temp->eng,
            temp->math,
            temp->total,
            temp->avg,
            temp->rank);
        temp = temp->next;
    }
    printf("\n");
}

/* ================= 메인 함수 ================= */
int main() {
    int menu;

    while (1) {
        printf("[Menu]\n");
        printf("1. .dat 파일에서 데이터 읽기\n");
        printf("2. 추가 학생 정보 입력\n");
        printf("3. .dat 파일 저장\n");
        printf("4. 성적 확인 (평균 계산 등)\n");
        printf("5. 종료\n");
        printf("----------------------------\n");
        printf("선택(1~5): ");
        scanf_s("%d", &menu);

        switch (menu) {
        case 1:
            loadFromFile();
            break;
        case 2:
            inputStudent();
            break;
        case 3:
            saveToFile();
            break;
        case 4:
            printStudents();
            break;
        case 5:
            printf("프로그램을 종료합니다.\n");
            return 0;
        default:
            printf("잘못된 선택입니다.\n\n");
        }
    }
}
