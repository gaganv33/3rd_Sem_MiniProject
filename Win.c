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
# define ENTER 2
# define TARGET 3
# define NEW 4

# define INTMAX 2147483647

int numFeatures;
int numExamples;
int lengthDataset;

// Storing the characters form the input box.
char InputDataset[10];
char InputTarget[10];
char features[10];
char examples[10];
char newExamples[10];

// Array used for storing the values.
int dataset[1000];
int target[1000];
int predict[1000];

int enteredExamples = 0;
int enteredFeatures = 0;
int counter = 0;
int target_counter = 0;
int new_example_count = 0;
float learning_rate = 0.005;

HWND hFeatures;
HWND hExamples;
HWND hDataset;
HWND hTarget;
HWND hNewExamples;


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

    CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 250, 50, 730, 750, NULL, NULL, NULL, NULL);

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
                    // Calling the function to do the rest of the calculations.
                    mainFunction(hWnd);
                    break;
                case ENTER:
                    if(counter == 0){
                        GetWindowTextW(hFeatures, features, 10);
                        GetWindowTextW(hExamples, examples, 10);

                        numFeatures = atoi(features);
                        numExamples = atoi(examples);

                        if(numExamples == 0 || numFeatures == 0){
                            // Displaying a message in case 
                            MessageBox(hWnd, "Number of features or examples cannot be zero.", NULL, NULL);
                        }
                    }

                    
                    GetWindowTextW(hDataset, InputDataset, 10);
                    int value = atoi(InputDataset);
                    // Appending the value to the array.
                    dataset[counter] = value;
                    counter = counter + 1;
                    // Updating the number of examples and features entered.
                    if(enteredFeatures % numFeatures == 0 && enteredFeatures != 0){
                        enteredExamples = enteredExamples + 1;
                        enteredFeatures = 0;
                    }
                    SetWindowTextW(hDataset, L"");

                    break;
                case TARGET:
                    GetWindowTextW(hTarget, InputTarget, 10);
                    int value_target = atoi(InputTarget);
                    // Appending the value to the array.
                    target[target_counter] = value_target;
                    target_counter = target_counter + 1;

                    SetWindowTextW(hTarget, L"");
                    break;
                case NEW:
                    GetWindowTextW(hNewExamples, newExamples, 10);
                    int value_new_example = atoi(newExamples);
                    // Appending the value to the array.
                    predict[new_example_count] = value_new_example;
                    new_example_count = new_example_count + 1;

                    SetWindowTextW(hNewExamples, L"");
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
    CreateWindowW(L"static", L"Linear Regression (Single Digit input examples and target examples)", WS_VISIBLE | WS_CHILD, 290, 25, 250, 50, hWnd, NULL, NULL, NULL);

    // Creating input box to input number of features
    CreateWindowW(L"static", L"Enter the number of features", WS_VISIBLE | WS_CHILD, 150, 130, 200, 30, hWnd, NULL, NULL, NULL);
    hFeatures = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_BORDER, 380, 130, 250, 30, hWnd,
    NULL, NULL, NULL);

    // Creating input box to input number of examples
    CreateWindowW(L"static", L"Enter the number of exampes", WS_VISIBLE | WS_CHILD, 150, 180, 200, 30, hWnd, NULL, NULL, NULL);
    hExamples = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_BORDER, 380, 180, 250, 30, hWnd, 
    NULL, NULL, NULL);

    // // Taking the input form the terminal.
    // CreateWindowW(L"static", L"Enter the training examples in the terminal", WS_VISIBLE | WS_CHILD, 150, 300, 400, 60, hWnd, NULL, NULL, NULL);

    // For inputting the training dataset.
    CreateWindowW(L"static", L"Enter the training examples", WS_VISIBLE | WS_CHILD, 150, 230, 200, 30, hWnd, NULL, NULL, NULL);
    hDataset = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_BORDER, 380, 230, 250, 35, hWnd, 
    NULL, NULL, NULL);
    CreateWindowW(L"button", L"Enter", WS_VISIBLE | WS_CHILD | WS_BORDER, 210, 250, 70, 30, hWnd, (HMENU)ENTER, NULL, NULL);

    // For inputting the target variables.
    CreateWindowW(L"static", L"Enter the target variables", WS_VISIBLE | WS_CHILD, 150, 290, 200, 30, hWnd, NULL, NULL, NULL);
    hTarget = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_BORDER, 380, 290, 250, 32, hWnd, 
    NULL, NULL, NULL);
    CreateWindowW(L"button", L"Enter", WS_VISIBLE | WS_CHILD | WS_BORDER, 210, 315, 70, 30, hWnd, (HMENU)TARGET, NULL, NULL);

    // For inputting the new examples for predict the target variable.
    CreateWindowW(L"static", L"Enter the new examples", WS_VISIBLE | WS_CHILD, 150, 355, 200, 30, hWnd, NULL, NULL, NULL);
    hNewExamples = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_BORDER, 380, 355, 250, 32, 
    hWnd, NULL, NULL, NULL);
    CreateWindowW(L"button", L"Enter", WS_VISIBLE | WS_CHILD | WS_BORDER, 210, 375, 70, 30, hWnd, (HMENU)NEW, NULL, NULL);

    // Using if block to display the output.
    CreateWindowW(L"button", L"Train The Parameters", WS_VISIBLE | WS_CHILD | WS_BORDER, 275, 570, 250, 50, hWnd, (HMENU)SUBMIT, NULL, 
    NULL);

}

void mainFunction(HWND hWnd){


    if(numHiddenLayers == 0){
        int count = 0;
        struct Node * head = (struct Node *) malloc(sizeof(struct Node));
        struct Node * tail = head;

        for(int i = 0; i < numExamples; i++){

            // Start of the linked list
            struct Node * start = (struct Node *) malloc(sizeof(struct Node));
            struct Node * end = start;

            // Main Linked List in empty
            if(i == 0){
                for(int j = 0; j < numFeatures; j++){

                    // Secondary Linked List is empty condition
                    if(j == 0){
                        start->data = dataset[count];
                        count = count + 1;
                    }
                    // Secondary Linked List is not empty
                    else{
                        struct Node * MiddleNode1 = (struct Node *) malloc(sizeof(struct Node));
                        MiddleNode1->data = dataset[count];
                        end->next = MiddleNode1;
                        end = MiddleNode1;
                        count = count + 1;
                    }
                }
                end->next = NULL;
                head->data = start;
            }

            else{
                // Creating a new node to insert it in the Main Linked List
                struct Node * NewNode = (struct Node *) malloc(sizeof(struct Node));
                for(int j = 0; j < numFeatures; j++){

                    // Secondary Linked List empty condition
                    if(j == 0){
                        start->data = dataset[count];
                        count = count + 1;
                    }
                    // Seconday Linked List is not empty
                    else{
                        struct Node * MiddleNode2 = (struct Node *) malloc(sizeof(struct Node));
                        MiddleNode2->data = dataset[count];
                        end->next = MiddleNode2;
                        end = MiddleNode2;
                        count = count + 1;
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

        // Printing the input examples for reference.
        printf("\nThe input examples are\n");
        while(headTemp1 != NULL){
            struct Node * startTemp1 = headTemp1->data;
            while(startTemp1 != NULL){
                printf("%d ", startTemp1->data);
                startTemp1 = startTemp1->next;
            }
            printf("\n");
            headTemp1 = headTemp1->next;
        }

        // Printing the target variables for reference.
        printf("\nThe target values are\n");
        for(int j = 0; j < numExamples; j++){
            printf("%d ", target[j]);
        }

        // Printing the new exmaples for refernce.
        printf("\nThe new exmaples are\n");
        for(int j = 0; j < numFeatures; j++){
            printf("%d ", predict[j]);
        }

        // Initializing parameters w and b
        float w[100];
        float b = 0.0;
        for(int i = 0; i < numFeatures; i++){
            w[i] = 0.0;
        }


        // Linear Regression Algorithm
        for(int i = 0;i < 1000; i++){
                
            // Gradient Descent
            // Initializing dw and db
            float dw[100];
            float db = 0.0;

            for(int j = 0; j < numFeatures; j++){
                dw[j] = 0.0;
            }

            // Finding the graient values
            int num_examples1 = 0;
            // Resetting the value of headTemp2 to head;
            headTemp2 = head;
            while(headTemp2 != NULL && num_examples1 < numExamples){
                float err = 0.0;
                struct Node * startTemp2 = headTemp2->data;

                // Calculating the value of the error of a single training example.
                int num_features1 = 0;
                while(startTemp2 != NULL && num_features1 < numFeatures){
                    // printf("\nInside Error: %d\n", startTemp2->data);
                    err = err + ((startTemp2->data) * w[num_features1]);
                    num_features1 = num_features1 + 1;
                    startTemp2 = startTemp2->next;
                }

                // Adding the value of b at the end.
                err = err +  b;

                // According to the formula.
                err = err - target[num_examples1];

                // Calculating the value of db
                db = db + err;

                // Calculating the value of dw
                struct Node * startTemp3 = headTemp2->data;
                int num_features2 = 0;
                while(startTemp3 != NULL && num_features2 < numFeatures){
                    // printf("Inside dw calculation: %d\n", startTemp3->data);
                    dw[num_features2] = dw[num_features2] + (err * (startTemp3->data));
                    startTemp3 = startTemp3->next;
                    num_features2 = num_features2 + 1;
                }
                // Updating the value of num_examples1 in order to compute err.
                num_examples1 = num_examples1 + 1;
                // Updating the value of headTemp2
                headTemp2 = headTemp2->next;
            }

            // Multiplying the parameters, w and b with (1 / examples)
            for(int a = 0; a < numFeatures; a++){
                dw[a] = dw[a] * pow(numExamples, -1);
                // printf("\nExamples^-1: %f", pow(numExamples, -1));
            }
            db = db * pow(numExamples, -1);

            // // Printing the value of db.
            // printf("\nThe value of db: %f\n", db);

            // Updating the values of parameters, w and b
            for(int a = 0; a < numFeatures; a++){
                w[a] = w[a] - (learning_rate * dw[a]);
            }
            b = b - (learning_rate * db);

            // Computing the value of the cost function.
            float cost = 0.0;
            // Resetting the value of the headTemp3 to head.
            headTemp3 = head;
            // Counter for the training example.
            int num_examples2 = 0;
            while(headTemp3 != NULL && num_examples2 < numExamples){
                struct Node * startTemp4 = headTemp3->data;

                // Value of the f = wx + b.
                float f_wb_i = 0.0;

                // Counter for features.
                int num_features3 = 0;

                while(startTemp4 != NULL && num_features3 < numFeatures){
                    // printf("Inside cost function calculation: %d\n", startTemp4->data);
                    f_wb_i = f_wb_i + ((startTemp4->data) * w[num_features3]);
                    startTemp4 = startTemp4->next;
                    num_features3 = num_features3 + 1;
                }
                // Adding b to f_wb_i
                f_wb_i = f_wb_i + b;
                // Computing the final cost function value.
                cost = cost + pow((f_wb_i - target[num_examples2]), 2);
                // printf("\nf_wb_i: %f\n", f_wb_i);

                // Updating the value of the num_examples2.
                num_examples2 = num_examples2 + 1;

                // Updating the value of headTemp3.
                headTemp3 = headTemp3->next;
            }
            cost = cost * pow((2 * numExamples), -1);

            printf("\nIteration: %d, Cost Function Value: %f", i + 1, cost);
        }
        printf("\nThe paramters w values are\n");
        for(int a = 0; a < numFeatures; a++){
            printf("w[%d]: %f\n", a, w[a]);
        }
        printf("\nThe paramters b value is: %f\n", b);
        float predicted = 0.0;
        for(int a = 0; a < numFeatures; a++){
            predicted = predicted + (w[a] * predict[a]);
        }
        printf("The predicted value for the new examples is: %f\n", predicted);

        
    }
}

