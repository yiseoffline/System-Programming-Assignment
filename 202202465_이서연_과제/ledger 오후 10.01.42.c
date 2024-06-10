#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ledger.h"

// 가계부에 항목을 추가하는 함수
void add_entry(const char *filename, const char *description, double amount) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    int id = rand() % 10000; // 랜덤 ID 생성
    fprintf(file, "%d,%s,%.2f\n", id, description, amount); // 파일에 항목 추가
    fclose(file);

    printf("항목 추가됨: ID=%d, 설명=%s, 금액=%.2f\n", id, description, amount);
}

// 가계부에서 항목을 삭제하는 함수
void delete_entry(const char *filename, int id) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    FILE *temp = fopen("temp.csv", "w");
    if (temp == NULL) {
        perror("fopen temp");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        int entry_id;
        char description[256];
        double amount;

        sscanf(line, "%d,%[^,],%lf", &entry_id, description, &amount);
        if (entry_id != id) {
            fprintf(temp, "%s", line); // 임시 파일에 항목 복사
        } else {
            found = 1; // 항목이 발견됨
        }
    }

    fclose(file);
    fclose(temp);

    remove(filename); // 원본 파일 삭제
    rename("temp.csv", filename); // 임시 파일을 원본 파일로 변경

    if (found) {
        printf("항목 삭제됨: ID=%d\n", id);
    } else {
        printf("항목을 찾을 수 없음: ID=%d\n", id);
    }
}

// 가계부의 모든 항목을 출력하는 함수
void view_entries(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char line[256];
    printf("ID, 설명, 금액\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line); // 파일에서 항목을 읽어 출력
    }

    fclose(file);
}

