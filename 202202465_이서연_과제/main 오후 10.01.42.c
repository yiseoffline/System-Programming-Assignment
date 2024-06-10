#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "ledger.h"
#include "system_info.h"

#define PIPE_READ 0
#define PIPE_WRITE 1

// 메뉴를 출력하는 함수
void print_menu() {
    printf("1. 항목 추가\n");
    printf("2. 항목 삭제\n");
    printf("3. 항목 보기\n");
    printf("4. 시스템 정보 보기\n");
    printf("5. 종료\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ledger_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int choice;
    char description[256];
    double amount;
    int id;

    while (1) {
        print_menu();
        printf("선택하세요: ");
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "입력을 읽지 못했습니다.\n");
            while (getchar() != '\n'); // 입력 버퍼 지우기
            continue;
        }

        switch (choice) {
            case 1: // 항목 추가
                printf("설명을 입력하세요: ");
                if (scanf("%255s", description) != 1) {
                    fprintf(stderr, "설명을 읽지 못했습니다.\n");
                    while (getchar() != '\n'); // 입력 버퍼 지우기
                    continue;
                }
                printf("금액을 입력하세요: ");
                if (scanf("%lf", &amount) != 1) {
                    fprintf(stderr, "금액을 읽지 못했습니다.\n");
                    while (getchar() != '\n'); // 입력 버퍼 지우기
                    continue;
                }
                add_entry(argv[1], description, amount); // 항목 추가 함수 호출
                break;
            case 2: // 항목 삭제
                printf("삭제할 항목 ID를 입력하세요: ");
                if (scanf("%d", &id) != 1) {
                    fprintf(stderr, "항목 ID를 읽지 못했습니다.\n");
                    while (getchar() != '\n'); // 입력 버퍼 지우기
                    continue;
                }
                delete_entry(argv[1], id); // 항목 삭제 함수 호출
                break;
            case 3: // 항목 보기
                view_entries(argv[1]); // 항목 보기 함수 호출
                break;
            case 4: { // 시스템 정보 보기
                int fd[2];
                if (pipe(fd) == -1) {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }

                pid_t pid = fork();
                if (pid == -1) {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }

                if (pid == 0) { // 자식 프로세스
                    close(fd[PIPE_READ]);
                    char info[1024];
                    get_system_info(info, sizeof(info)); // 시스템 정보 수집
                    if (write(fd[PIPE_WRITE], info, sizeof(info)) == -1) {
                        perror("write");
                        exit(EXIT_FAILURE);
                    }
                    close(fd[PIPE_WRITE]);
                    execl("/bin/ls", "ls", "-l", NULL); // 외부 프로그램 실행
                    perror("execl");
                    exit(EXIT_FAILURE);
                } else { // 부모 프로세스
                    close(fd[PIPE_WRITE]);
                    char buffer[1024];
                    if (read(fd[PIPE_READ], buffer, sizeof(buffer)) == -1) {
                        perror("read");
                        exit(EXIT_FAILURE);
                    }
                    close(fd[PIPE_READ]);
                    wait(NULL);
                    printf("자식 프로세스에서 받은 시스템 정보:\n%s\n", buffer);
                }
                break;
            }
            case 5: // 종료
                exit(0);
            default:
                printf("잘못된 선택입니다. 다시 시도하세요.\n");
        }
    }

    return 0;
}

