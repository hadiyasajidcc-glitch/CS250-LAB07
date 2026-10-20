// Lab 8 Task 2 
// students are ordered by studentID
#include <iostream>
#include <string>
using namespace std;

struct Student {
    int studentID;
    string name;
    float GPA;
    Student* left;
    Student* right;
};

void printStudent(Student* s) {
    cout << "ID: " << s->studentID << ", Name: " << s->name
         << ", GPA: " << s->GPA << endl;
}

// smaller id goes left, bigger goes right
// time complexity: O(log n) average, O(n) worst
Student* insertStudent(Student* root, int id, string name, float gpa) {
    // empty place found so make the node here
    if (root == NULL) {
        Student* s = new Student;
        s->studentID = id;
        s->name = name;
        s->GPA = gpa;
        s->left = NULL;
        s->right = NULL;
        return s;
    }

    if (id < root->studentID) {
        root->left = insertStudent(root->left, id, name, gpa);
    } else if (id > root->studentID) {
        root->right = insertStudent(root->right, id, name, gpa);
    }
    return root;
}

// returns NULL if student is not found
// time complexity: O(log n) average, O(n) worst
Student* searchStudent(Student* root, int id) {
    if (root == NULL || root->studentID == id) {
        return root;
    }
    if (id < root->studentID) {
        return searchStudent(root->left, id);
    }
    return searchStudent(root->right, id);
}

// smallest node is the left most one
Student* findMin(Student* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

// time complexity: O(log n) average, O(n) worst
Student* deleteStudent(Student* root, int id) {
    if (root == NULL) return NULL;

    if (id < root->studentID) {
        root->left = deleteStudent(root->left, id);
    } else if (id > root->studentID) {
        root->right = deleteStudent(root->right, id);
    } else {
        // found it

        // no child or one child
        if (root->left == NULL) {
            Student* temp = root->right;
            delete root;
            return temp;
        }
        if (root->right == NULL) {
            Student* temp = root->left;
            delete root;
            return temp;
        }

        // two children, copy the smallest node of right side
        // here and then delete that node
        Student* minNode = findMin(root->right);
        root->studentID = minNode->studentID;
        root->name = minNode->name;
        root->GPA = minNode->GPA;
        root->right = deleteStudent(root->right, minNode->studentID);
    }
    return root;
}

// inorder gives students sorted by id
// time complexity: O(n)
void inorder(Student* root) {
    if (root == NULL) return;
    inorder(root->left);
    printStudent(root);
    inorder(root->right);
}

// time complexity: O(n)
int countStudents(Student* root) {
    if (root == NULL) return 0;
    return 1 + countStudents(root->left) + countStudents(root->right);
}

// have to check every node because tree is sorted by id not gpa
// time complexity: O(n)
void findHighLow(Student* root, Student*& high, Student*& low) {
    if (root == NULL) return;
    if (high == NULL || root->GPA > high->GPA) high = root;
    if (low == NULL || root->GPA < low->GPA) low = root;
    findHighLow(root->left, high, low);
    findHighLow(root->right, high, low);
}

// time complexity: O(n)
void printAbove(Student* root, float threshold) {
    if (root == NULL) return;
    printAbove(root->left, threshold);
    if (root->GPA > threshold) {
        printStudent(root);
    }
    printAbove(root->right, threshold);
}

void freeTree(Student* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    Student* root = NULL;

    // adding some students first for testing
    root = insertStudent(root, 105, "Ali", 3.5);
    root = insertStudent(root, 102, "Sara", 3.8);
    root = insertStudent(root, 110, "Ahmed", 2.9);
    root = insertStudent(root, 101, "Zara", 3.2);
    root = insertStudent(root, 104, "Hamza", 2.5);
    root = insertStudent(root, 108, "Fatima", 3.9);
    root = insertStudent(root, 115, "Usman", 3.0);

    int choice;
    do {
        cout << "1. Insert student" << endl;
        cout << "2. Search student" << endl;
        cout << "3. Delete student" << endl;
        cout << "4. Display all students" << endl;
        cout << "5. Count students" << endl;
        cout << "6. Highest and lowest GPA" << endl;
        cout << "7. Students above a GPA" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int id;
            string name;
            float gpa;
            cout << "Enter ID: ";
            cin >> id;
            cout << "Enter name: ";
            cin >> name;
            cout << "Enter GPA: ";
            cin >> gpa;

            if (searchStudent(root, id) != NULL) {
                cout << "This ID already exists" << endl;
            } else {
                root = insertStudent(root, id, name, gpa);
                cout << "Student added" << endl;
            }
        }
        else if (choice == 2) {
            int id;
            cout << "Enter ID: ";
            cin >> id;
            Student* found = searchStudent(root, id);
            if (found == NULL) {
                cout << "Student not found" << endl;
            } else {
                printStudent(found);
            }
        }
        else if (choice == 3) {
            int id;
            cout << "Enter ID: ";
            cin >> id;
            if (searchStudent(root, id) == NULL) {
                cout << "Student not found" << endl;
            } else {
                root = deleteStudent(root, id);
                cout << "Student deleted" << endl;
            }
        }
        else if (choice == 4) {
            if (root == NULL) {
                cout << "No students" << endl;
            } else {
                inorder(root);
            }
        }
        else if (choice == 5) {
            cout << "Total students: " << countStudents(root) << endl;
        }
        else if (choice == 6) {
            Student* high = NULL;
            Student* low = NULL;
            findHighLow(root, high, low);
            if (high == NULL) {
                cout << "No students" << endl;
            } else {
                cout << "Highest GPA: ";
                printStudent(high);
                cout << "Lowest GPA: ";
                printStudent(low);
            }
        }
        else if (choice == 7) {
            float t;
            cout << "Enter GPA: ";
            cin >> t;
            printAbove(root, t);
        }
        else if (choice != 0) {
            cout << "Wrong choice" << endl;
        }
    } while (choice != 0);

    freeTree(root);
    return 0;
}