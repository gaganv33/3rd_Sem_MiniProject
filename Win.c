#include<Windows.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>

struct Node{
    int data;
    struct Node * next;
};

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void AddControls(HWND hWnd);
void mainFunction(HWND hWnd);

# define SUBMIT 1

int numFeatures;
int numExamples;
int numHiddenLayers;

char features[10];
char examples[10];
char hiddenLayers[10];

HWND hFeatures;
HWND hExamples;
HWND hHiddenLayers;


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdsow){

    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = WindowProcedure;

    if(!RegisterClassW(&wc)){
        return -1;
    }

    CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 250, 50, 750, 750, NULL, NULL, NULL, NULL);

    MSG msg = {0};

    while( GetMessage(&msg, NULL, NULL, NULL) ){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp){
    switch(msg){
        case WM_COMMAND:
            switch(wp){
                case SUBMIT:
                    GetWindowTextW(hFeatures, features, 10);
                    GetWindowTextW(hExamples, examples, 10);
                    GetWindowTextW(hHiddenLayers, hiddenLayers, 10);

                    numHiddenLayers = atoi(hiddenLayers);
                    numFeatures = atoi(features);
                    numExamples = atoi(examples);

                    if(numExamples == 0 || numFeatures == 0){
                        MessageBox(hWnd, "Number of features or examples cannot be zero.", NULL, NULL);
                    }

                    mainFunction(hWnd);
                    break;
            }
            break;
        case WM_CREATE:
            AddControls(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hWnd, msg, wp, lp);
    }
}

void AddControls(HWND hWnd){
    CreateWindowW(L"static", L"Linear Regression", WS_VISIBLE | WS_CHILD, 325, 25, 200, 50, hWnd, NULL, NULL, NULL);\

    // Creating input box to input number of hidden layers
    CreateWindowW(L"static", L"Enter the number of hidden Layers", WS_VISIBLE | WS_CHILD, 150, 80, 200, 30, hWnd, NULL, NULL, NULL);
    hHiddenLayers = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_BORDER, 380, 80, 250, 30, hWnd, NULL, 
    NULL, NULL);

    // Creating input box to input number of features
    CreateWindowW(L"static", L"Enter the number of features", WS_VISIBLE | WS_CHILD, 150, 140, 200, 30, hWnd, NULL, NULL, NULL);
    hFeatures = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_BORDER, 380, 140, 250, 30, hWnd,
    NULL, NULL, NULL);

    // Creating input box to input number of examples
    CreateWindowW(L"static", L"Enter the number of exampes", WS_VISIBLE | WS_CHILD, 150, 200, 200, 30, hWnd, NULL, NULL, NULL);
    hExamples = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_BORDER, 380, 190, 250, 30, hWnd, NULL, NULL, 
    NULL);

    // Taking the input form the terminal.
    CreateWindowW(L"static", L"Enter the training examples in the terminal", WS_VISIBLE | WS_CHILD, 150, 300, 400, 60, hWnd, NULL, NULL, NULL);

    // Using if block to display the output.

    CreateWindowW(L"button", L"Train The Parameters", WS_VISIBLE | WS_CHILD | WS_BORDER, 275, 570, 250, 50, hWnd, (HMENU)SUBMIT, NULL, NULL);
}

void mainFunction(HWND hWnd){
    printf("Enter the dataset seperated by a space\n");
    
    struct Node * head = (struct Node *) malloc(sizeof(struct Node));
    struct Node * tail = head;

    // Inputing the data from the user. And Storing it in the linked list.
    for(int i = 0; i < numExamples; i++){
        struct Node * start = (struct Node *) malloc(sizeof(struct Node));
        struct Node * end = start;
            
        // Main Linked List empty condition
        if(i == 0){
            for(int j = 0; j < numFeatures; j++){
                
                // Secondary Linked List empty condition
                if(j == 0){
                    int train;
                    scanf("%d", &train);
                    start->data = train;
                }

                else{
                    struct Node * MiddleNode = (struct Node *) malloc(sizeof(struct Node));
                    int train;
                    scanf("%d", &train);
                    MiddleNode->data = train;
                    end->next = MiddleNode;
                    end = MiddleNode;
                }
            }
            end->next = NULL;
            head->data = start;
        }
        // Main Linked List is not empty
        else{
            // Creating a new Node to insert it in the Main Linked List.
            struct Node * NewNode = (struct Node *) malloc(sizeof(struct Node));
            for(int j = 0; j < numFeatures; j++){

                // Secondary Linked List empty condition
                if(j == 0){
                    int train;
                    scanf("%d", &train);
                    start->data = train;
                }

                else{
                    struct Node * MiddleNode2 = (struct Node *) malloc(sizeof(struct Node));
                    int train;
                    scanf("%d", &train);
                    MiddleNode2->data = train;
                    end->next = MiddleNode2;
                    end = MiddleNode2;
                }
            }
            end->next = NULL;
            NewNode->data = start;
            tail->next = NewNode;
            tail = NewNode;
        }
    }
    tail->next = NULL;

    // Creating temporary head variables 
    // headTemp1 used for printing the training examples for reference.
    struct Node * headTemp1 = head;
    // headTemp2 used for the calculation of err.
    struct Node * headTemp2 = head;
    // headTemp3 used for the calculation of the cost function.
    struct Node * headTemp3 = head;

    // Printing the training dataset.
    printf("\nThe training dataset is\n");

    while(headTemp1 != NULL){
        struct Node * startTemp1 = headTemp1->data;
        while(startTemp1 != NULL){
            printf("%d ", startTemp1->data);
            startTemp1 = startTemp1->next;
        }
        printf("\n");
        headTemp1 = headTemp1->next;
    }
}