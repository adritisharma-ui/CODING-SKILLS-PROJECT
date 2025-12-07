/*
    Student Record Management System (Single File Version - Windows Optimized)
    Features: Admin (Manage Users), Teacher (Manage Marks), Student (View Marks)
    Persistence: Text Files
*/

#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>


#define sleep(x) Sleep(1000 * (x))

// ==========================================
// MODELS & CONFIG
// ==========================================

#define MAX_NAME 100
#define MAX_EMAIL 100
#define MAX_PASS 50

#define FILE_USERS "users.txt"
#define FILE_STUDENTS "students.txt"
#define FILE_TEACHERS "teachers.txt"
#define FILE_MARKS "marks.txt"

typedef enum { ROLE_ADMIN = 1, ROLE_TEACHER = 2, ROLE_STUDENT = 3 } UserRole;

typedef struct {
  int id;
  char email[MAX_EMAIL];
  char password[MAX_PASS];
  int role;
} User;

typedef struct {
  int id;
  int user_id;
  char name[MAX_NAME];
  char roll_no[20];
  int class_id;
} StudentProfile;

typedef struct {
  int id;
  int user_id;
  char name[MAX_NAME];
  char subject[50];
} TeacherProfile;

typedef struct {
  int id;
  int student_id;
  char subject[50];
  float score;
} Marks;

// ==========================================
// UTILS
// ==========================================

void clearScreen() { system("cls"); }

void printHeader(const char *title) {
  clearScreen();
  printf("==================================================\n");
  printf("      STUDENT RECORD MANAGEMENT SYSTEM\n");
  printf("==================================================\n");
  printf(">> %s\n", title);
  printf("--------------------------------------------------\n");
}

void trimWhitespace(char *str) {
  char *end;
  while (isspace((unsigned char)*str))
    str++;
  if (*str == 0)
    return;
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
    end--;
  *(end + 1) = 0;
}

void pauseWait() {
  printf("\nPress any key to continue...");
  _getch();
}

int getLine(char *buffer, int size) {
  if (fgets(buffer, size, stdin) != NULL) {
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    }
    trimWhitespace(buffer);
    return 1;
  }
  return 0;
}

void appendLine(const char *filename, const char *line) {
  FILE *fp = fopen(filename, "a");
  if (fp) {
    fprintf(fp, "%s\n", line);
    fclose(fp);
  }
}

// ==========================================
// FILE I/O
// ==========================================

void initFiles() {
  FILE *fp;
  const char *files[] = {FILE_USERS, FILE_STUDENTS, FILE_TEACHERS, FILE_MARKS};
  for (int i = 0; i < 4; i++) {
    fp = fopen(files[i], "r");
    if (!fp) {
      fp = fopen(files[i], "w");
    }
    if (fp)
      fclose(fp);
  }
}

int saveUser(User u) {
  char line[512];
  sprintf(line, "%d|%s|%s|%d", u.id, u.email, u.password, u.role);
  appendLine(FILE_USERS, line);
  return 1;
}

int getAllUsers(User users[], int max_count) {
  FILE *fp = fopen(FILE_USERS, "r");
  if (!fp)
    return 0;

  char line[512];
  int count = 0;
  while (fgets(line, sizeof(line), fp) && count < max_count) {
    trimWhitespace(line);
    if (strlen(line) == 0)
      continue;

    char *token = strtok(line, "|");
    if (!token)
      continue;
    users[count].id = atoi(token);

    token = strtok(NULL, "|");
    if (!token)
      continue;
    strcpy(users[count].email, token);

    token = strtok(NULL, "|");
    if (!token)
      continue;
    strcpy(users[count].password, token);

    token = strtok(NULL, "|");
    if (!token)
      continue;
    users[count].role = atoi(token);

    count++;
  }
  fclose(fp);
  return count;
}

User getUserByEmail(const char *email) {
  User u = {0};
  User users[100];
  int count = getAllUsers(users, 100);
  for (int i = 0; i < count; i++) {
    if (strcmp(users[i].email, email) == 0) {
      return users[i];
    }
  }
  return u;
}

int saveStudentProfile(StudentProfile s) {
  char line[512];
  sprintf(line, "%d|%d|%s|%s|%d", s.id, s.user_id, s.name, s.roll_no,
          s.class_id);
  appendLine(FILE_STUDENTS, line);
  return 1;
}

int getStudentProfileByUserId(int user_id, StudentProfile *out_s) {
  FILE *fp = fopen(FILE_STUDENTS, "r");
  if (!fp)
    return 0;
  char line[512];
  while (fgets(line, sizeof(line), fp)) {
    trimWhitespace(line);
    if (strlen(line) == 0)
      continue;

    StudentProfile s;
    char *token = strtok(line, "|");
    if (!token)
      continue;
    s.id = atoi(token);

    token = strtok(NULL, "|");
    if (!token)
      continue;
    s.user_id = atoi(token);

    token = strtok(NULL, "|");
    if (!token)
      continue;
    strcpy(s.name, token);

    token = strtok(NULL, "|");
    if (!token)
      continue;
    strcpy(s.roll_no, token);

    token = strtok(NULL, "|");
    if (!token)
      continue;
    s.class_id = atoi(token);

    if (s.user_id == user_id) {
      *out_s = s;
      fclose(fp);
      return 1;
    }
  }
  fclose(fp);
  return 0;
}

int getAllStudentProfiles(StudentProfile s[], int max_count) {
  FILE *fp = fopen(FILE_STUDENTS, "r");
  if (!fp)
    return 0;
  char line[512];
  int count = 0;
  while (fgets(line, sizeof(line), fp) && count < max_count) {
    trimWhitespace(line);
    if (strlen(line) == 0)
      continue;

    char *token = strtok(line, "|");
    if (!token)
      continue;
    s[count].id = atoi(token);

    token = strtok(NULL, "|");
    if (!token)
      continue;
    s[count].user_id = atoi(token);

    token = strtok(NULL, "|");
    if (!token)
      continue;
    strcpy(s[count].name, token);

    token = strtok(NULL, "|");
    if (!token)
      continue;
    strcpy(s[count].roll_no, token);

    token = strtok(NULL, "|");
    if (!token)
      continue;
    s[count].class_id = atoi(token);

    count++;
  }
  fclose(fp);
  return count;
}

int saveTeacherProfile(TeacherProfile t) {
  char line[512];
  sprintf(line, "%d|%d|%s|%s", t.id, t.user_id, t.name, t.subject);
  appendLine(FILE_TEACHERS, line);
  return 1;
}

int saveMarks(Marks m) {
  char line[512];
  sprintf(line, "%d|%d|%s|%.2f", m.id, m.student_id, m.subject, m.score);
  appendLine(FILE_MARKS, line);
  return 1;
}

int getMarksByStudentId(int student_id, Marks out_marks[], int max_count) {
  FILE *fp = fopen(FILE_MARKS, "r");
  if (!fp)
    return 0;
  char line[512];
  int count = 0;
  while (fgets(line, sizeof(line), fp) && count < max_count) {
    trimWhitespace(line);
    if (strlen(line) == 0)
      continue;

    char *token = strtok(line, "|");
    if (!token)
      continue;
    int id = atoi(token);

    token = strtok(NULL, "|");
    if (!token)
      continue;
    int s_id = atoi(token);

    if (s_id == student_id) {
      out_marks[count].id = id;
      out_marks[count].student_id = s_id;

      token = strtok(NULL, "|");
      if (!token)
        continue;
      strcpy(out_marks[count].subject, token);

      token = strtok(NULL, "|");
      if (!token)
        continue;
      out_marks[count].score = atof(token);

      count++;
    }
  }
  fclose(fp);
  return count;
}

// ==========================================
// AUTH MODULE
// ==========================================

User *login(const char *email, const char *password) {
  static User u;
  u = getUserByEmail(email);
  if (u.id != 0) {
    if (strcmp(u.password, password) == 0) {
      return &u;
    }
  }
  return NULL;
}

// ==========================================
// ADMIN MODULE
// ==========================================

void addTeacher() {
  printHeader("ADD NEW TEACHER");
  char name[MAX_NAME], email[MAX_EMAIL], pass[MAX_PASS], subject[50];

  printf("Name: ");
  getLine(name, MAX_NAME);
  printf("Email: ");
  getLine(email, MAX_EMAIL);
  printf("Password: ");
  getLine(pass, MAX_PASS);
  printf("Subject: ");
  getLine(subject, 50);

  User u;
  User users[500];
  int count = getAllUsers(users, 500);
  u.id = count + 1000 + (rand() % 1000);
  strcpy(u.email, email);
  strcpy(u.password, pass);
  u.role = ROLE_TEACHER;
  saveUser(u);

  TeacherProfile tp;
  tp.id = u.id + 5000;
  tp.user_id = u.id;
  strcpy(tp.name, name);
  strcpy(tp.subject, subject);
  saveTeacherProfile(tp);

  printf("\nTeacher added successfully!\n");
  pauseWait();
}

void addStudent() {
  printHeader("ADD NEW STUDENT");
  char name[MAX_NAME], email[MAX_EMAIL], pass[MAX_PASS], roll[20];
  int class_id;
  char buffer[20];

  printf("Name: ");
  getLine(name, MAX_NAME);
  printf("Email: ");
  getLine(email, MAX_EMAIL);
  printf("Password: ");
  getLine(pass, MAX_PASS);
  printf("Roll No: ");
  getLine(roll, 20);
  printf("Class ID (int): ");
  getLine(buffer, 20);
  class_id = atoi(buffer);

  User u;
  User users[500];
  int count = getAllUsers(users, 500);
  u.id = count + 1000 + (rand() % 1000);
  strcpy(u.email, email);
  strcpy(u.password, pass);
  u.role = ROLE_STUDENT;
  saveUser(u);

  StudentProfile sp;
  sp.id = u.id + 5000;
  sp.user_id = u.id;
  strcpy(sp.name, name);
  strcpy(sp.roll_no, roll);
  sp.class_id = class_id;
  saveStudentProfile(sp);

  printf("\nStudent added successfully!\n");
  pauseWait();
}

void adminMenu(User adminUser) {
  int choice;
  do {
    printHeader("ADMIN DASHBOARD");
    printf("1. Manage Teachers (Add)\n");
    printf("2. Manage Students (Add)\n");
    printf("0. Logout\n");
    printf("--------------------------------------------------\n");
    printf("Enter Choice: ");

    char buffer[100];
    getLine(buffer, 100);
    choice = atoi(buffer);

    switch (choice) {
    case 1:
      addTeacher();
      break;
    case 2:
      addStudent();
      break;
    case 0:
      break;
    default:
      printf("Invalid Choice!\n");
      pauseWait();
    }
  } while (choice != 0);
}

// ==========================================
// TEACHER MODULE
// ==========================================

void manageMarks(User teacher) {
  printHeader("MANAGE MARKS");

  // 1. List all students first
  StudentProfile students[100];
  int count = getAllStudentProfiles(students, 100);

  if (count == 0) {
    printf("No students found in the system.\n");
    pauseWait();
    return;
  }

  printf("\nAvailable Students:\n");
  printf("%-20s | %-10s\n", "Name", "Roll No");
  printf("-------------------------------------\n");
  for (int i = 0; i < count; i++) {
    printf("%-20s | %-10s\n", students[i].name, students[i].roll_no);
  }
  printf("-------------------------------------\n");

  char searchName[MAX_NAME];
  printf("Enter Student Name: ");
  getLine(searchName, MAX_NAME);

  // 2. Search in the loaded list (Case Insensitive)
  int foundIndex = -1;
  for (int i = 0; i < count; i++) {
    if (_stricmp(students[i].name, searchName) == 0) {
      foundIndex = i;
      break;
    }
  }

  if (foundIndex == -1) {
    printf("Student '%s' not found!\n", searchName);
    pauseWait();
    return;
  }

  StudentProfile s = students[foundIndex];
  printf("Selected Student: %s (Roll: %s)\n", s.name, s.roll_no);

  char subject[50];
  char buffer[100];
  float score;

  printf("Subject: ");
  getLine(subject, 50);

  printf("Score: ");
  getLine(buffer, 100);
  score = atof(buffer);

  Marks m;
  m.id = rand();
  m.student_id = s.user_id; // Map to User ID
  strcpy(m.subject, subject);
  m.score = score;

  saveMarks(m);
  printf("Marks uploaded successfully!\n");
  pauseWait();
}

void teacherMenu(User teacherUser) {
  int choice;
  do {
    printHeader("TEACHER DASHBOARD");
    printf("1. Manage Marks\n");
    printf("0. Logout\n");
    printf("--------------------------------------------------\n");
    printf("Enter Choice: ");

    char buffer[100];
    getLine(buffer, 100);
    choice = atoi(buffer);

    switch (choice) {
    case 1:
      manageMarks(teacherUser);
      break;
    case 0:
      break;
    default:
      printf("Invalid Choice!\n");
      pauseWait();
    }
  } while (choice != 0);
}

// ==========================================
// STUDENT MODULE
// ==========================================

void viewMarks(User student) {
  printHeader("MY MARKS");

  StudentProfile s;
  if (!getStudentProfileByUserId(student.id, &s)) {
    printf("Profile not found!\n");
    pauseWait();
    return;
  }

  Marks marks[100];
  int count = getMarksByStudentId(s.user_id, marks, 100);

  if (count == 0) {
    printf("No marks available.\n");
  } else {
    printf("%-20s | %-10s\n", "Subject", "Score");
    printf("-----------------------------------\n");
    for (int i = 0; i < count; i++) {
      printf("%-20s | %.2f\n", marks[i].subject, marks[i].score);
    }
  }
  pauseWait();
}

void studentMenu(User studentUser) {
  int choice;
  do {
    printHeader("STUDENT DASHBOARD");
    printf("1. View Marks\n");
    printf("0. Logout\n");
    printf("--------------------------------------------------\n");
    printf("Enter Choice: ");

    char buffer[100];
    getLine(buffer, 100);
    choice = atoi(buffer);

    switch (choice) {
    case 1:
      viewMarks(studentUser);
      break;
    case 0:
      break;
    default:
      printf("Invalid Choice!\n");
      pauseWait();
    }
  } while (choice != 0);
}

// ==========================================
// MAIN
// ==========================================

int main() {
  srand((unsigned)time(NULL)); // Seed random
  initFiles();

  User u = getUserByEmail("admin");
  if (u.id == 0) {
    printf("Initializing default Admin account...\n");
    User admin;
    admin.id = 1;
    strcpy(admin.email, "admin");
    strcpy(admin.password, "admin");
    admin.role = ROLE_ADMIN;
    saveUser(admin);
    sleep(1);
  }

  while (1) {
    printHeader("LOGIN");

    char email[100];
    char password[100];

    printf("Email: ");
    getLine(email, 100);

    printf("Password: ");
    getLine(password, 100);

    User *loggedInUser = login(email, password);

    if (loggedInUser) {
      printf("\nLogin Successful! Welcome, %s\n", loggedInUser->email);
      sleep(1);

      switch (loggedInUser->role) {
      case ROLE_ADMIN:
        adminMenu(*loggedInUser);
        break;
      case ROLE_TEACHER:
        teacherMenu(*loggedInUser);
        break;
      case ROLE_STUDENT:
        studentMenu(*loggedInUser);
        break;
      default:
        printf("Error: Unknown Role.\n");
        pauseWait();
      }
    } else {
      printf("\nInvalid Credentials! Try again.\n");
      sleep(2);
    }
  }

  return 0;
}
