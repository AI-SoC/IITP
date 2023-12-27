
int main(){
    int i;
    int num1,num2,num3;
    num1=1;
    num2=1;

    for(i=0;i<6;i++){
         num3=num2+num1;
         num2=num1;
         num1=num3;
    }
}

