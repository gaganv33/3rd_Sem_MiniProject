#include<stdio.h>
#include<stdlib.h>
#include<math.h>

struct Node{
    int data;
    struct Node * next;
};

int main(){
    int number_of_hidden_layers;
    printf("Enter the number of hidden layers\n");
    scanf("%d", &number_of_hidden_layers);

    if(number_of_hidden_layers == 0){

        int features, examples;

        printf("Enter the total number of features\n");
        scanf("%d", &features);

        printf("Enter the total number of training examples\n");
        scanf("%d", &examples);

        struct Node * head = (struct Node *) malloc(sizeof(struct Node));
        struct Node * tail = head;

        printf("Enter the training data seperated by a space\n");

        // Inputing the data from the user. And Storing it in the linked list.
        for(int i = 0; i < examples; i++){
            struct Node * start = (struct Node *) malloc(sizeof(struct Node));
            struct Node * end = start;
            
            // Main Linked List empty condition
            if(i == 0){
                for(int j = 0; j < features; j++){
                    
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
                for(int j = 0; j < features; j++){

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

        // Inputing the target variable
        printf("\nEnter the target values for %d training examples.\n", examples);
        int targetVariable[100];
        for (int i = 0; i < examples; i++){
            scanf("%d", &targetVariable[i]);
        }

        // Printing the target variable for the training dataset.
        printf("\nThe target variable for the training dataset\n");
        for(int i = 0;i < examples; i++){
            printf("%d ", targetVariable[i]);
        }

        // Inputing the number of iterations and the learning rate
        int num_iterations;
        float learning_rate;

        printf("\nEnter the total number of iterations for gradient descent\n");
        scanf("%d", &num_iterations);

        printf("\nEnter the learning rate value for updating the parameters\n");
        scanf("%f", &learning_rate);

        printf("\nNumber of Iterations: %d, Learning Rate: %f\n", num_iterations, learning_rate);

        // Initializing parameters w and b
        float w[100];
        float b = 0.0;
        for(int i = 0; i < features; i++){
            w[i] = 0.0;
        }

        // Linear Regression Algorithm
        for(int i = 0;i < num_iterations; i++){
            
            // Gradient Descent
            // Initializing dw and db
            float dw[100];
            float db = 0.0;

            for(int j = 0; j < features; j++){
                dw[j] = 0.0;
            }

            // Finding the graient values
            int num_examples1 = 0;
            // Resetting the value of headTemp2 to head;
            headTemp2 = head;
            while(headTemp2 != NULL && num_examples1 < examples){
                float err = 0.0;
                struct Node * startTemp2 = headTemp2->data;

                // Calculating the value of the error of a single training example.
                int num_features1 = 0;
                while(startTemp2 != NULL && num_features1 < features){
                    // printf("\nInside Error: %d\n", startTemp2->data);
                    err = err + ((startTemp2->data) * w[num_features1]);
                    num_features1 = num_features1 + 1;
                    startTemp2 = startTemp2->next;
                }

                // Adding the value of b at the end.
                err = err +  b;

                // According to the formula.
                err = err - targetVariable[num_examples1];

                // Calculating the value of db
                db = db + err;

                // Calculating the value of dw
                struct Node * startTemp3 = headTemp2->data;
                int num_features2 = 0;
                while(startTemp3 != NULL && num_features2 < features){
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
            for(int a = 0; a < features; a++){
                dw[a] = dw[a] * pow(examples, -1);
                printf("\nExamples^-1: %f", pow(examples, -1));
            }
            db = db * pow(examples, -1);

            // Updating the values of parameters, w and b
            for(int a = 0; a < features; a++){
                w[a] = w[a] - (learning_rate * dw[a]);
            }
            b = b - (learning_rate * db);

            // Computing the value of the cost function.
            float cost = 0.0;
            // Resetting the value of the headTemp3 to head.
            headTemp3 = head;
            // Counter for the training example.
            int num_examples2 = 0;
            while(headTemp3 != NULL && num_examples2 < examples){
                struct Node * startTemp4 = headTemp3->data;

                // Value of the f = wx + b.
                float f_wb_i = 0.0;

                // Counter for features.
                int num_features3 = 0;

                while(startTemp4 != NULL && num_features3 < features){
                    // printf("Inside cost function calculation: %d\n", startTemp4->data);
                    f_wb_i = f_wb_i + ((startTemp4->data) * w[num_features3]);
                    startTemp4 = startTemp4->next;
                    num_features3 = num_features3 + 1;
                }
                // Adding b to f_wb_i
                f_wb_i = f_wb_i + b;
                // Computing the final cost function value.
                cost = cost + pow((f_wb_i - targetVariable[num_examples2]), 2);
                printf("\nf_wb_i: %f\n", f_wb_i);

                // Updating the value of the num_examples2.
                num_examples2 = num_examples2 + 1;

                // Updating the value of headTemp3.
                headTemp3 = headTemp3->next;
            }
            cost = cost * pow((2 * examples), -1);

            // cost = cost * pow((2 * examples), -1);
            printf("\nIteration: %d, Cost Function Value: %f", i + 1, cost);
        }
    }
    else{
        printf("\nMore than 0 hidden Layers is a neural network.\n");
    }
    return 0;
}