#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
/*
 First completed at April 25,2016
 Totally Completed at April 26,2016
 Combined all.
 Made by @ MaoQuan
 */
using namespace std;

ifstream fin("/Users/mouizumi/Desktop/编译原理实验/Grammer/text.txt ");

string binary;

int tpn = 0;
int truenum = -1;
int falsenum= -1001;
bool times = false;
bool plusy = false;
bool recentt = false;
bool com = false;
bool k = false;
bool inelse = false;
bool infl = false;
bool stop;
bool inboolk = false;
bool have = false;
bool already = false;
string nowtrue;
string nowfalse;
string it;

class fouritem{
public:
    string opreater;
    string item1;
    string item2;
    string aim;
};

class record{
public:
    map<string,int>table;
    void putin(string name,int num){
        table[name] = num;
    }
    void print(){
        cout<<endl;
        cout<<"It's the beiginning of the table!"<<endl;
        for(auto s:table){
            cout<<s.first<<" "<<s.second<<endl;
        }
    }
    void buildkey(int M = -1){
        //自动建立保留字表
        if(M == -1){
            table["program"] = 25;
            table["var"] = 26;
            table["procedure"] = 27;
            table["begin"] = 28;
            table["end"] = 29;
            table["if"] = 30;
            table["then"] = 31;
            table["else"] = 32;
            table["while"] = 33;
            table["do"] = 34;
            table["call"] = 35;
            table["integer"] = 36;
            table["real"] = 37;
        }else{
            string tm;int t = 0;
            while(M--){
                cin>>tm;
                table[tm] = t;
                t++;
            }
        }
    }
    int match(string key){
        // int j=0;
        for(auto s:table){
            if(key == s.first) return table[key];
        }
        return -1;
    }
    int mat(string key){
        int j=0;
        for(auto s:table){
            if(key == s.first) return j;
            j++;
        }
        table[key] = j;
        return j;
    }
    
};

//四元式表
vector<fouritem> fitems;
vector<fouritem> tempsforr;
pair<int,int> judge;
vector<string> um;
stack<string> ope;
record idtablea,keytable;   //idtable:标识符表；keytable:保留字表

void scanner(FILE *fp);
bool F1();
bool F();
bool E();
bool I();
bool give();
bool program();
bool subprogram();
bool explain();
bool sentences();
bool explaintable();
bool idtable();
bool boolexpression();
bool relationexpression();
bool condition();
bool sentn();
bool loop();
void pushinbook(string op,string it1,string it2,string aim);
void printoutbook();
void afterask();
string askforfalse();
string claimfortrue();
string askfortrue();
string claimforfalse();

void scanner(FILE *fp){
    char ch;
    // FILE *fpw = fopen("out.txt","w");
    while((ch = fgetc(fp))!=EOF){
        string token="";
        int c;
        if(isalpha(ch)){    //identifier or key
            token+=ch;
            ch = fgetc(fp);
            while(isalnum(ch)){
                token+=ch;
                ch = fgetc(fp);
            }
            if(ch!=EOF)fseek(fp,-1,1);
            c = keytable.match(token);
            if(c==-1){
                c = idtablea.mat(token);
                cout<<"("<<0<<","<<c<<")"<<" "<<token<<endl;      //identifier
            }else{
                cout<<"("<<c<<","<<0<<")"<<" "<<token<<endl;  //keyword
            }
        }else if(isdigit(ch)){      //QUESTION!
            token+=ch;
            //bool fl = false;
            ch = fgetc(fp);
            while(isdigit(ch)){
                token+=ch;
                ch = fgetc(fp);
            }
            if(isalpha(ch)) {
                while(isalpha(ch)){
                    ch = fgetc(fp);
                }
                fseek(fp,-1,1);
                cout<<"ERROR:ILLEGAL INDENTIFIER!"<<endl;
            }
            else{
                fseek(fp,-1,1);
                cout<<"("<<1<<","<<token<<")"<<" "<<token<<endl;
            }
        }else{
            switch(ch){
                case '+':
                    cout<<"("<<3<<","<<"0"<<")"<<" +"<<endl;
                    break;
                case '-':
                    cout<<"("<<4<<","<<"0"<<")"<<" -"<<endl;
                    break;
                case '*':
                    cout<<"("<<5<<","<<"0"<<")"<<" *"<<endl;
                    break;
                case '/':
                    //解决注释问题
                    ch = fgetc(fp);
                    if(ch=='*'){
                        ch = fgetc(fp);
                        //cout<<"("<<20<<","<<"/*"<<")"<<" /*"<<endl;
                        ch = fgetc(fp);
                        while(1){
                            if(ch=='*'){
                                ch = fgetc(fp);
                                if(ch=='/') {
                                    // cout<<"("<<21<<","<<"*/"<<")"<<" */"<<endl;
                                    break;
                                }
                                if(ch==EOF) {cout<<"no end of comment"<<endl;break;}
                                else continue;
                            }else if(ch == EOF)  {cout<<"ERROR!no end of comment"<<endl;break;}
                            else ch = fgetc(fp);
                        }
                    }else if(ch == '/'){
                        while(1){
                            ch = fgetc(fp);
                            if(ch == '\n' || ch == '\r') break;
                        }
                        // cout<<"("<<22<<","<<"//"<<")"<<" //"<<endl;
                    }else {
                        fseek(fp,-1, 1);
                        cout<<"("<<6<<","<<"0"<<")"<<" /"<<endl;
                    }
                    break;
                case '<':
                    ch = fgetc(fp);
                    if(ch == '=') cout<<"("<<13<<","<<"0"<<")"<<" <="<<endl;//question:二元式后面部分是不是输出这个
                    else if(ch=='>') cout<<"("<<15<<","<<"0"<<")"<<" <>"<<endl;
                    else{
                        fseek(fp,-1,1);
                        cout<<"("<<8<<","<<"0"<<")"<<" <"<<endl;
                    }
                    break;
                case '>':
                    ch = fgetc(fp);
                    if(ch == '=') cout<<"("<<14<<","<<"0"<<")"<<" >="<<endl;
                    else{
                        fseek(fp,-1,1);
                        cout<<"("<<7<<","<<"0"<<")"<<" >"<<endl;
                    }
                    break;
                case '(':
                    cout<<"("<<9<<","<<"0"<<")"<<" ("<<endl;
                    break;
                case ')':
                    cout<<"("<<10<<","<<"0"<<")"<<" )"<<endl;
                    break;
                case '~':
                    cout<<"("<<11<<","<<"0"<<")"<<" ~"<<endl;
                    break;
                case '=':
                    cout<<"("<<17<<","<<"0"<<")"<<" ="<<endl;
                    break;
                case ':':
                    ch = fgetc(fp);
                    if(ch == '='){
                        cout<<"("<<12<<","<<"0"<<")"<<" :="<<endl;
                    }else{
                        cout<<"("<<38<<","<<"0"<<")"<<" :"<<endl;
                        fseek(fp,-1,1);
                    }
                    break;
                case ';':
                    cout<<"("<<17<<","<<"0"<<")"<<" ;"<<endl;
                    break;
                case ',':
                    cout<<"("<<18<<","<<"0"<<")"<<" ,"<<endl;
                    break;
                case '.':
                    cout<<"("<<19<<","<<"0"<<")"<<" ."<<endl;
                    break;
                case ' ':
                    break;
                case '\n':
                    break;
                case '\r':
                    break;
                case '\t':
                    break;
                case '|':
                    cout<<"("<<23<<","<<"0"<<")"<<" |"<<endl;
                    break;
                case '&':
                    cout<<"("<<24<<","<<"0"<<")"<<" &"<<endl;
                    break;
                case '!':
                    cout<<"("<<40<<","<<"0"<<")"<<" !"<<endl;
                    break;
                default:
                    cout<<"ERROR:no such keyword or an illegal identifier:"<<" "<<ch<<endl;
                    break;
            }
        }
    }
}

void read(){
    getline(fin,binary);
    stringstream ss(binary);
    int x,y;
    char t1,t2;
    ss>>t1>>x>>t2>>y>>t1;
    judge.first = x;
    judge.second = y;
    ss>>it;
}

string ClaimForTemp(){
    tpn++;
    return "T"+to_string(tpn);
}

bool program(){
    read();
    //cout<<judge.first<<" "<<judge.second<<endl;
    if(judge.first == 25){
        read();
        if(judge.first == 0){
            read();
            if(judge.first == 17){
                if(subprogram()){
                    return true;
                }else{
                    return false;
                }
            }else{
                cout<<"ERROR 1:";
                cout<<"need a semicolon"<<endl;
                return false;
            }
        }else{
            cout<<"ERROR 2:";
            cout<<"need an id"<<endl;
            return false;
        }
    }else{
        cout<<"ERROR 3:";
        cout<<"need a string of PROGRAM"<<endl;
        return false;
    }
}

bool subprogram(){
    if(explain()){
        // read();
        // if(judge.first == 17){
        // read();
        if(judge.first == 28){
            if(sentn()){
                // read();
                if(judge.first == 29){
                    //return true;
                    read();
                    if(judge.first == 19){
                        return true;
                    }else{
                        cout<<"ERROR 4:";
                        cout<<"NEED A ."<<endl;
                        return false;
                    }
                }else{
                    cout<<"ERROR 5:";
                    cout<<"need a string of END"<<endl;
                    return false;
                }
            }else{
                //cout<<"ERROR 6:";
                return false;
            }
        }else{
            cout<<"ERROR 7:";
            cout<<"need a string of BEGIN"<<endl;
            return false;
        }
    }
    else{
        cout<<"ERROR 8:";
        cout<<2<<endl;
        return false;
    }
}

bool explain(){
    read();
    if(judge.first == 26){
        if(explaintable()){
            return true;
        }else{
            return false;
        }
    }else{
        cout<<"ERROR 10:";
        cout<<"NEED A STRING OF VAR"<<endl;
        return false;
    }
}

bool explaintable(){
    //read();
    if(idtable()){
        // read();
        if(judge.first == 38){
            read();
            if(judge.first == 36){
                read();
                if(judge.first == 17){
                    //mark = fin.tellg();
                    if(explaintable()){//何时结束？回退？
                        return true;
                    }else{
                        // fin.seekg(mark);   //回退
                        return true ;
                    }
                }else{
                    cout<<"ERROR 11:";
                    cout<<"NEED A SEMICOLON"<<endl;
                    return false;
                }
            }else{
                cout<<"ERROR 12:";
                cout<<"NEED A SORT"<<endl;
                return false;
            }
        }else{
            cout<<"ERROR 13:";
            cout<<"NEED A COMMON"<<endl;
            return false;
        }
    }else{
        return false;
    }
}

bool idtable(){
    read();
    if(judge.first == 0){
        read();
        if(judge.first == 18){
            if(idtable()){
                return true;
            }else{
                cout<<"ERROR 15:";
                cout<<"ONE MORE COLON"<<endl;
                return false;
            }
        }else{
            return true;
        }
    }else{
        return false;
    }
}


bool complex(){
    
    // read();
    com = true;
    if(judge.first == 28){
        if(sentn()){
            //read();
            if(judge.first == 29){
                read();
                return true;
            }else{
                cout<<"ERROR 43:";
                cout<<"NEED AN END"<<endl;
                return false;
            }
        }else{
            cout<<"ERROR 44:";
            return false;
        }
    }else{
        cout<<"ERROR 45:";
        cout<<"NEED A BEGIN"<<endl;
        return false;
    }
    return false;
}

bool give(){
    //fouritem t;
    //read();
    if(judge.first == 0){
        string aim =it;
        read();
        if(judge.first ==12){
            //ope.push(it);
            //read();
            stop = false;
            if(F()){
                string it1 = ope.top();
                ope.pop();
                pushinbook(":=",it1," ",aim);
                return true;
            }else{
                cout<<"ERROR 40:";
                cout<<"NEED AN EXPRESSION"<<endl;
                return false;
            }
        }else{
            cout<<"ERROR 41:";
            cout<<"NEED A :="<<endl;
            return false;
        }
    }else{
        cout<<"ERROR 42:";
        cout<<"NEED AN ID"<<endl;
        return false;
    }
}

bool E(){
    if(I()){
        //read();
        if(judge.first == 3){
            if(I()){
                //read();
                return true;
            }
        }else{
            return true;
        }
    }
    return false;
}

bool I(){
    if(F()){
        //read();
        if(judge.first == 5){
            if(F()){
                //read();
                return true;
            }
        }else{
            return true;
        }
    }
    return false;
}


bool sentn(){
    if(sentences()){
        // read();
        if(judge.first == 17){
            if(sentn()){
                //read()?
                return true;
            }else{
                cout<<"ERROR 18:";
                cout<<"NEED A SENTENCE"<<endl;
            }
        }else{
            // read();
            return true;
        }
    }
    // cout<<"ERROR 19:";
    return false;
}
bool f = false;
bool sentences(){
    read();
    if(judge.first == 30){
        if(condition()){
            if(!inelse){
            string t5 = to_string(fitems.size()+1);
            string ask = askforfalse();
            for(int j=0;j<fitems.size();j++){
                if(fitems[j].aim == ask) fitems[j].aim=t5;
            }
            falsenum++;
            }
            inelse = false;
            return true;
        }
    }else if(judge.first == 0){
        if(give()){
            return true;
        }
    }else if(judge.first == 33){
        if(loop()){
            //printoutbook();
            string t5 = to_string(fitems.size()+1);
            string ask = askforfalse();
            for(int j=0;j<fitems.size();j++){
                if(fitems[j].aim == ask) {fitems[j].aim=t5;f=true;}
            }
            if(f) falsenum++;
            f = false;
            //printoutbook();
            return true;
        }
    }else if(judge.first == 28){
        if(complex()){
            return true;
        }
    }
    return false;
}

bool F(){
    if(!stop)read();
    stop = false;
    if(judge.first == 9){
        k = true;
        if(times) recentt = true;
        times = false;
        if(F()){
            stop = false;
            if(judge.first == 10){
                if(recentt){
                    string k =ClaimForTemp();
                    string t1 = ope.top();
                    ope.pop();
                    string t2 = ope.top();
                    ope.pop();
                    pushinbook("*",t2,t1,k);
                    ope.push(k);
                    times = false;
                }
                if(!stop) read();
                stop = false;
                k = false;
                if(F1()){
                    return true;
                }
            }else{
                cout<<"ERROR 52:NEED A )"<<endl;
                return false;
            }
        }else{
            cout<<"ERROR 53: NEED AN EXPRESSION"<<endl;
            return false;
        }
    }
    if(judge.first == 1){
        if(times){
            string k =ClaimForTemp();
            pushinbook("*",ope.top(),it,k);
            ope.pop();
            ope.push(k);
            times = false;
        }else{
            ope.push(it);
        }
        
        if(!stop) read();
        stop = false;
        if(judge.first!=5 && judge.first !=3) {stop=true;return true;}  //only number
        if(F1()){
            if(stop && ope.size()>=2){
                string k =ClaimForTemp();
                string t1 = ope.top();
                ope.pop();
                string t2 = ope.top();
                ope.pop();
                pushinbook("+",t2,t1,k);
                ope.push(k);
            }
            return true;
        }else{
            cout<<"ERROR 54:NEED AN EXPRESSION "<<endl;
            return false;
        }
    }
    if(judge.first == 0){
        if(times){
            string k =ClaimForTemp();
            pushinbook("*",ope.top(),it,k);
            ope.pop();
            ope.push(k);
            times = false;
        }else{
            ope.push(it);
        }
        
        if(!stop) read();
        stop = false;
        if(judge.first!=5 && judge.first !=3) {stop=true;return true;}  //only number
        if(F1()){
            if(stop && ope.size()>=2){
                string k =ClaimForTemp();
                string t1 = ope.top();
                ope.pop();
                string t2 = ope.top();
                ope.pop();
                pushinbook("+",t2,t1,k);
                ope.push(k);
            }

            return true;
        }else{
            cout<<"ERROR 54:NEED AN EXPRESSION "<<endl;
            return false;
        }
    }
    return false;
}

bool F1(){
    //read();
    if(judge.first == 5){
        times = true;
        if(F()){
            
            if(!stop) read();
            stop = false;
            if(judge.first!=5 && judge.first !=3) {stop=true;return true;}
            if(F1()){
                return true;
            }else{
                cout<<"ERROR:NEED AN EXPREESION"<<endl;
                return false;
            }
        }else{
            cout<<"ERROR:NEED AN EXPREESION"<<endl;
            return false;
        }
    }
    if(judge.first == 3){
        if(ope.size()>=2 && !k){
            string k =ClaimForTemp();
            string t1 = ope.top();
            ope.pop();
            string t2 = ope.top();
            ope.pop();
            pushinbook("+",t2,t1,k);
            ope.push(k);
        }

        if(F()){
            if(!stop) read();
            stop = false;
            if(judge.first!=5 && judge.first !=3) {stop=true;return true;}
            if(F1()){
                return true;
            }else{
                cout<<"ERROR:NEED AN EXPREESION"<<endl;
            }
        }else{
            cout<<"ERROR:NEED AN EXPREESION"<<endl;
            return false;
        }
    }
    return true;
}

bool relation(){
    // read();
    if(judge.first == 7||judge.first == 8 || judge.first == 9 || judge.first == 17 || judge.first == 13 || judge.first == 14 || judge.first == 15){
        return true;
    }
    //cout<<"ERROR 35:";
    return false;
}

string claimfortrue(){
    truenum--;
    return to_string(truenum);
}
string askfortrue(){
    return to_string(truenum);
}
string askforfalse(){
    return to_string(falsenum);
}
string claimforfalse(){
    falsenum--;
    return to_string(falsenum);
}
void afterask(){
    truenum++;
    falsenum++;
}

bool boolexpression(){
    if(relationexpression()){
        //read();
        if(judge.first == 24){
            read();
            if(judge.first == 24){
                if(inboolk && !already){
                    claimfortrue();
                    claimforfalse();
                    nowfalse = askforfalse();
                    nowtrue = askfortrue();
                    already = true;
                }
                have = true;
                string t5= to_string(fitems.size()+1);
                string ask = askfortrue();
                for(int j=0;j<fitems.size();j++){
                    if(fitems[j].aim == ask) fitems[j].aim=t5;
                }
                    afterask();
                if(boolexpression()){
                    return true;
                }else{
                    cout<<"ERROR 26:";
                    cout<<"NEED A BOOLEXPRESSION"<<endl;
                    return false;
                }
            }else{
                cout<<"ERROR 27:";
                cout<<"NEED A &"<<endl;
                return false;
            }
        }else if(judge.first == 23){
            read();
            if(judge.first == 23){
                have = true;
                string t5= to_string(fitems.size()+1);
                string ask = askforfalse();
                for(int j=0;j<fitems.size();j++){
                    if(fitems[j].aim == ask ) fitems[j].aim=t5;
                }
                afterask();
                if(boolexpression()){
                    return true;
                }else{
                    cout<<"ERROR 28:";
                    cout<<"NEED A BOOLEXPRESSION"<<endl;
                    return false;
                }
            }else{
                cout<<"ERROR 29:";
                cout<<"NEED A | "<<endl;
                return false;
            }
        }else{
            return true;
        }
    }
    read();
    return false;
}

bool relationexpression(){
    stop = false;
    read();
    if(judge.first==9){
        inboolk = true;
        have = false;
        already = false;
        if(boolexpression()){
            if(judge.first == 10){
                if(have){
                    string t5= nowfalse;
                string ask = askforfalse();
                for(int j=0;j<fitems.size();j++){
                    if(fitems[j].aim == ask ) fitems[j].aim=t5;
                }
                t5 = nowtrue;
                ask = askfortrue();
                for(int j=0;j<fitems.size();j++){
                    if(fitems[j].aim == ask) fitems[j].aim=t5;
                }
                    afterask();
                }
                read();
                inboolk = false;
                return true;
            }
        }
    }
    if(judge.first==0 || judge.first == 1){
        stop = true;
        if(F()){
            if(relation()){
                string t1 = ope.top();
                ope.pop();
                string t3 = "j"+it;
                stop = false;
                if(F()){
                    if(inboolk && (judge.first == 23 || judge.first == 24) && !already){
                        claimfortrue();
                        claimforfalse();
                        nowfalse = askforfalse();
                        nowtrue = askfortrue();
                        already = true;
                    }
                    string t2 = ope.top();
                    ope.pop();
                    pushinbook(t3,t1,t2,claimfortrue());
                    pushinbook("j"," "," ",claimforfalse());
                    return true;
                }else{
                    cout<<"ERROR 32:";
                    cout<<"NEED AN EXPRESSION"<<endl;
                    return false;
                }
            }else{
                string t1 = ope.top();
                ope.pop();
                stop = false;
                pushinbook("jnz",t1," ",claimfortrue());
                pushinbook("j"," "," ",claimforfalse());
                return true;
            }
        }
    }
    if(judge.first == 40){
        int nowit = (int)fitems.size();
        infl = relationexpression();
        for(int j=nowit;j<fitems.size();j++){
            if(stoi((fitems[j].aim))==falsenum){
                fitems[j].aim = to_string(truenum);
            }else if(stoi((fitems[j].aim))==truenum){
                fitems[j].aim = to_string(falsenum);
            }
        }
        return infl;
    }else{
        cout<<"ERROR 34:";
        cout<<"NEED AN EXPRESSION"<<endl;
        return false;
    }
}




bool condition(){
    // read();
    if(judge.first == 30){
        if(boolexpression()){
            //read();
            if(judge.first == 31){
                string t5= to_string(fitems.size()+1);
                string ask = askfortrue();
                for(int j=0;j<fitems.size();j++){
                    if(fitems[j].aim == ask) fitems[j].aim=t5;
                }
                truenum++;
             if(sentences()){
                 if(judge.first == 29 && com) {
                       return true;
                   }
                if(judge.first == 32){
                        inelse = true;
                        pushinbook("j"," "," ",claimfortrue());
                        string t5= to_string(fitems.size()+1);
                        string ask = askforfalse();
                        for(int j=0;j<fitems.size();j++){
                            if(fitems[j].aim == ask) fitems[j].aim=t5;
                        }
                        falsenum++;
                       if(sentences()){
                           string t5 = to_string(fitems.size()+1);
                           string ask = askfortrue();
                           for(int j=0;j<fitems.size();j++){
                               if(fitems[j].aim == ask) fitems[j].aim=t5;
                           }
                           truenum++;
                        return true;
                        }else{
                            cout<<"ERROR 48:";
                            cout<<"NEED A SENTENCE"<<endl;
                        }
                    }else{
                        return true;
                    }
                }else{
                    cout<<"ERROR 46:";
                    return false;
                }
            }else{
                cout<<"ERROR 49";
                cout<<"NEED THEN"<<endl;
            }
        }else{
            cout<<"ERROR 50:";
            cout<<"NEED A BOOLEXPRESSION"<<endl;
        }
    }else{
        cout<<"ERROR 51:";
        cout<<"NEED AN IF"<<endl;
    }
    return false;
}

bool loop(){
    //read();
    if(judge.first == 33){
        string loopstart = to_string(fitems.size()+1);
        if(boolexpression()){
            //read();
            if(judge.first == 34){
                string t5= to_string(fitems.size()+1);
                string ask = askfortrue();
                for(int j=0;j<fitems.size();j++){
                    if(fitems[j].aim == ask) fitems[j].aim=t5;
                }
                truenum++;
                if(sentences()){
                    pushinbook("j"," ", " ", loopstart);
                    string t5= to_string(fitems.size());
                    for(int j=0;j<fitems.size();j++){
                        if(fitems[j].aim == t5) fitems[j].aim=loopstart;
                    }
                    return true;
                }else{
                    cout<<"ERROR 36:";
                    cout<<"NEED AN SENTENCES"<<endl;
                    return false;
                }
            }else{
                cout<<"ERROR 37:";
                cout<<"LESS OF DO"<<endl;
                return false;
            }
        }else{
            cout<<"ERROR 38:";
            cout<<"NEED A BOOLEXPRESSION"<<endl;
            return false;
        }
    }else{
        cout<<"ERROR 39:";
        cout<<"NEED A WHILE"<<endl;
        return false;
    }
}


void pushinbook(string op,string it1,string it2,string aim){
    fouritem tmp;
    tmp.opreater = op;
    tmp.item1 = it1;
    tmp.item2 = it2;
    tmp.aim = aim;
    fitems.push_back(tmp);
//    printoutbook();
//    cout<<endl;
}


void printoutbook(){
    int num=1;
    for(auto s:fitems){
        cout<<num<<". ("<<s.opreater<<", "<<s.item1<<", "<<s.item2<<", "<<s.aim<<")"<<endl;
        num++;
    }
}

int main(void){
    freopen("/Users/mouizumi/Desktop/编译原理实验/Grammer/text.txt ","w",stdout);
    keytable.buildkey();
    FILE* fp = fopen("/Users/mouizumi/Desktop/编译原理实验/text2.txt","r");
    //judge(fp);
    scanner(fp);
    idtablea.print();
    fclose(fp);
    freopen("/Users/mouizumi/Desktop/编译原理实验/four.txt ","w",stdout);
    if(program()){
    pushinbook("ret"," "," ","0");
    printoutbook();
    }else{
        cout<<"There are some errors in GRAMMER"<<endl;
    }
    return 0;
}

