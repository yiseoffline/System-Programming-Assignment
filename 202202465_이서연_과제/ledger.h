#ifndef LEDGER_H
#define LEDGER_H

// 가계부에 항목을 추가하는 함수 선언
void add_entry(const char *filename, const char *description, double amount);

// 가계부에서 항목을 삭제하는 함수 선언
void delete_entry(const char *filename, int id);

// 가계부의 모든 항목을 출력하는 함수 선언
void view_entries(const char *filename);

#endif // LEDGER_H

