#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DATA_FILE "accounts.dat"
#define NAME_LEN 50

typedef struct {
    int acc_no;
    char name[NAME_LEN];
    int pin;           /* simple PIN for basic verification */
    double balance;
} Account;

/* ---------- Input helpers ---------- */
static void flushInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

static int readInt(const char *prompt) {
    int val;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &val) == 1) {
            flushInput();
            return val;
        } else {
            printf("Invalid input. Enter an integer.\n");
            flushInput();
        }
    }
}

static double readDouble(const char *prompt) {
    double val;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf", &val) == 1) {
            flushInput();
            return val;
        } else {
            printf("Invalid input. Enter a number.\n");
            flushInput();
        }
    }
}

static void readString(const char *prompt, char *out, size_t len) {
    printf("%s", prompt);
    if (fgets(out, len, stdin) == NULL) {
        out[0] = '\0';
        return;
    }
    out[strcspn(out, "\n")] = '\0'; /* trim newline */
}

/* ---------- File utilities ---------- */

static int get_next_account_number(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) return 1001; /* start account numbers from 1001 */

    Account a;
    int last = 1000;
    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.acc_no > last) last = a.acc_no;
    }
    fclose(fp);
    return last + 1;
}

static bool write_account(const Account *a) {
    FILE *fp = fopen(DATA_FILE, "ab");
    if (!fp) {
        perror("Error opening data file");
        return false;
    }
    if (fwrite(a, sizeof(*a), 1, fp) != 1) {
        perror("Write error");
        fclose(fp);
        return false;
    }
    fclose(fp);
    return true;
}

/* find account by account number. returns true and fills 'out' if found */
static bool find_account(int acc_no, Account *out) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) return false;
    Account a;
    bool found = false;
    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.acc_no == acc_no) {
            if (out) *out = a;
            found = true;
            break;
        }
    }
    fclose(fp);
    return found;
}

/* rewrite full file from temp array (used by update/delete) */
static bool rewrite_all_accounts_excluding(int exclude_acc_no, const Account *replace, bool do_replace) {
    /* reads all accounts, writes to temp either skipping exclude or replacing it */
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) return false;
    FILE *tf = fopen("temp_accounts.dat", "wb");
    if (!tf) {
        fclose(fp);
        perror("Error creating temp file");
        return false;
    }

    Account a;
    bool replaced = false;
    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.acc_no == exclude_acc_no) {
            if (do_replace && replace) {
                if (fwrite(replace, sizeof(*replace), 1, tf) != 1) {
                    perror("Temp write error");
                    fclose(fp); fclose(tf); remove("temp_accounts.dat");
                    return false;
                }
                replaced = true;
            } else {
                /* skip copying -> effectively delete */
            }
        } else {
            if (fwrite(&a, sizeof(a), 1, tf) != 1) {
                perror("Temp write error");
                fclose(fp); fclose(tf); remove("temp_accounts.dat");
                return false;
            }
        }
    }

    fclose(fp);
    fclose(tf);

    /* replace original file */
    if (remove(DATA_FILE) != 0) {
        /* if remove fails because file doesn't exist, still try rename */
        /* ignore error */
    }
    if (rename("temp_accounts.dat", DATA_FILE) != 0) {
        perror("Error renaming temp file");
        return false;
    }

    return do_replace ? replaced : true;
}

/* ---------- Bank operations ---------- */

static void create_account(void) {
    Account a;
    a.acc_no = get_next_account_number();
    readString("\nEnter account holder name: ", a.name, sizeof(a.name));
    a.pin = readInt("Set 4-digit PIN (numbers only): ");
    if (a.pin < 0) a.pin = -a.pin;
    a.balance = readDouble("Enter initial deposit amount: ");
    if (a.balance < 0) a.balance = 0.0;

    if (write_account(&a)) {
        printf("\nAccount created successfully!\n");
        printf("Account Number: %d\n", a.acc_no);
    } else {
        printf("Failed to create account.\n");
    }
}

static bool verify_pin(const Account *a) {
    int pin = readInt("Enter PIN: ");
    return pin == a->pin;
}

static void deposit(void) {
    int acc = readInt("\nEnter account number: ");
    Account a;
    if (!find_account(acc, &a)) {
        printf("Account not found.\n");
        return;
    }
    double amt = readDouble("Enter amount to deposit: ");
    if (amt <= 0) {
        printf("Invalid amount.\n");
        return;
    }
    a.balance += amt;

    if (!rewrite_all_accounts_excluding(acc, &a, true)) {
        printf("Error updating account.\n");
    } else {
        printf("Deposit successful. New balance: %.2lf\n", a.balance);
    }
}

static void withdraw(void) {
    int acc = readInt("\nEnter account number: ");
    Account a;
    if (!find_account(acc, &a)) {
        printf("Account not found.\n");
        return;
    }
    if (!verify_pin(&a)) {
        printf("PIN verification failed.\n");
        return;
    }
    double amt = readDouble("Enter amount to withdraw: ");
    if (amt <= 0) {
        printf("Invalid amount.\n");
        return;
    }
    if (amt > a.balance) {
        printf("Insufficient funds. Current balance: %.2lf\n", a.balance);
        return;
    }
    a.balance -= amt;

    if (!rewrite_all_accounts_excluding(acc, &a, true)) {
        printf("Error updating account.\n");
    } else {
        printf("Withdrawal successful. New balance: %.2lf\n", a.balance);
    }
}

static void transfer(void) {
    int src = readInt("\nEnter your account number (source): ");
    Account a_src;
    if (!find_account(src, &a_src)) {
        printf("Source account not found.\n");
        return;
    }
    if (!verify_pin(&a_src)) {
        printf("PIN verification failed.\n");
        return;
    }

    int dst = readInt("Enter destination account number: ");
    Account a_dst;
    if (!find_account(dst, &a_dst)) {
        printf("Destination account not found.\n");
        return;
    }

    double amt = readDouble("Enter amount to transfer: ");
    if (amt <= 0) { printf("Invalid amount.\n"); return; }
    if (amt > a_src.balance) { printf("Insufficient funds.\n"); return; }

    a_src.balance -= amt;
    a_dst.balance += amt;

    /* update both accounts */
    if (!rewrite_all_accounts_excluding(src, &a_src, true)) {
        printf("Error updating source account.\n");
        return;
    }
    if (!rewrite_all_accounts_excluding(dst, &a_dst, true)) {
        printf("Error updating destination account.\n");
        return;
    }

    printf("Transfer successful. New balance (source): %.2lf\n", a_src.balance);
}

static void display_account(void) {
    int acc = readInt("\nEnter account number: ");
    Account a;
    if (!find_account(acc, &a)) {
        printf("Account not found.\n");
        return;
    }
    printf("\n---- Account Details ----\n");
    printf("Account Number : %d\n", a.acc_no);
    printf("Name           : %s\n", a.name);
    printf("Balance        : %.2lf\n", a.balance);
}

static void display_all_accounts(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        printf("\nNo accounts found.\n");
        return;
    }
    printf("\n====== ALL ACCOUNTS ======\n");
    Account a;
    int count = 0;
    while (fread(&a, sizeof(a), 1, fp) == 1) {
        printf("\nAccount: %d | Name: %s | Balance: %.2lf\n", a.acc_no, a.name, a.balance);
        count++;
    }
    if (count == 0) printf("No accounts to show.\n");
    fclose(fp);
}

static void delete_account(void) {
    int acc = readInt("\nEnter account number to delete: ");
    Account a;
    if (!find_account(acc, &a)) {
        printf("Account not found.\n");
        return;
    }
    if (!verify_pin(&a)) {
        printf("PIN verification failed.\n");
        return;
    }
    if (!rewrite_all_accounts_excluding(acc, NULL, false)) {
        printf("Error deleting account.\n");
    } else {
        printf("Account %d deleted successfully.\n", acc);
    }
}

static void update_account(void) {
    int acc = readInt("\nEnter account number to update: ");
    Account a;
    if (!find_account(acc, &a)) {
        printf("Account not found.\n");
        return;
    }
    if (!verify_pin(&a)) {
        printf("PIN verification failed.\n");
        return;
    }

    printf("\nWhat do you want to update?\n");
    printf("1. Name\n2. PIN\n3. Cancel\n");
    int ch = readInt("Enter choice: ");
    if (ch == 1) {
        readString("Enter new name: ", a.name, sizeof(a.name));
    } else if (ch == 2) {
        a.pin = readInt("Enter new PIN: ");
    } else {
        printf("Update cancelled.\n");
        return;
    }

    if (!rewrite_all_accounts_excluding(acc, &a, true)) {
        printf("Error updating account.\n");
    } else {
        printf("Account updated successfully.\n");
    }
}

/* ---------- Main menu ---------- */

int main(void) {
    printf("===== Simple Bank Management System =====\n");
    while (1) {
        printf("\n1. Create Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Transfer\n");
        printf("5. Display Account\n");
        printf("6. Display All Accounts\n");
        printf("7. Update Account (name/PIN)\n");
        printf("8. Delete Account\n");
        printf("9. Exit\n");

        int choice = readInt("Enter choice: ");
        switch (choice) {
            case 1: create_account(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: transfer(); break;
            case 5: display_account(); break;
            case 6: display_all_accounts(); break;
            case 7: update_account(); break;
            case 8: delete_account(); break;
            case 9: printf("Goodbye!\n"); return 0;
            default: printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
