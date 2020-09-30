#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<map>
using namespace std;


//KMP algorithm
bool KMP(string,string);
//For prefix table KMP
void prefix(string);
//Comparison function KMP handles special cases
bool compare(char,char,bool);
//Readlines from sample file
map<int,string> readline(string);
//Breaks -ivw
vector<string> breakcommand(string command, int* isCorrectCommand);
//Handles cases of i v w
bool casehandler(string);

//setting variable based on cases
bool ignorecases=false;//i
bool invert=false;//v
bool linenumprint=false;//n
bool addblankswhilecheck=false;//w

//for check blank ',' '.'
bool isblankorextra(char);
//function for grep instruction
bool grepfunction(string command);

int F[1000000];

bool debug=false;

string convertword(string a)
{
    string b=" "+a+" ";
    return b;
}
void testcaserun();


int main()
{
    string str;
    //testcaserun();
    //freopen("input.txt","r",stdin);
    //freopen("output2.txt","w",stdout);

    while(getline(cin,str))
    {
        //initialising
        ignorecases=false;
        invert=false;
        linenumprint=false;
        addblankswhilecheck=false;

        //Running grep function
        grepfunction(str);
		if(debug)
		cout<<"\n";
    }
}


//For testcase ignore
void testcaserun()
{
    vector<string> commandfull;
    vector<string>::iterator itr;
	
	
	
    string command="line sample.txt";
    commandfull.push_back(command);
	
	command="\"line\" sample.txt";
    commandfull.push_back(command);
	
	command="\"line is\" sample.txt";
    commandfull.push_back(command);
	
	command="line is sample.txt";
    commandfull.push_back(command);
	
	command="-w \"line is\" sample.txt";
    commandfull.push_back(command);
	
	command="-w \" line is \" sample.txt";
    commandfull.push_back(command);
	
	
	
    command="-v line sample.txt";
    commandfull.push_back(command);
    command="-i line sample.txt";
    commandfull.push_back(command);
    command="-w line sample.txt";
    commandfull.push_back(command);
    command="-n line sample.txt";
    commandfull.push_back(command);
    command="-iw line sample.txt";
    commandfull.push_back(command);
    command="-i line sample.txt";
    for(itr=commandfull.begin(); itr!=commandfull.end(); itr++)
    {
        grepfunction(*itr);
        ignorecases=false;//
        invert=false;
        linenumprint=false;
        addblankswhilecheck=false;
    }
}

//Grep function
bool grepfunction(string command)
{	
	int isCorrectCommand=1;
    vector<string> commands=breakcommand(command,&isCorrectCommand);
	
	if(!isCorrectCommand)
		return -1;

    vector<string>::iterator itr;
    //break command
    for(itr=commands.begin(); itr!=commands.end(); itr++)
    {
        string code=*itr;

        if(debug)
            cout<<*itr<<"\n";

        //If - found
        if(code.find('-')!=string::npos)
        {
            bool flag=casehandler(code);
            if(flag==false)
                return false;
        }
    }
    //For finding file name
    itr--;

    map<int,string> lines=readline(*itr);

    //For finding word
    itr--;

    string word=*(itr);

    if(debug)
        cout<<"word="<<word<<"\n";

    map<int,string>::iterator linesitr;
    map<int,string> answer;
    map<int,string>::iterator ansiterator;

    bool ans;
    // For individual lines
    for(linesitr=lines.begin(); linesitr!=lines.end(); linesitr++)
    {

        if(!addblankswhilecheck)
            ans=KMP(linesitr->second,word);
        //adding blank before after pattern for -w case
        else
            ans=KMP(linesitr->second,convertword(word));

        if(!invert)
        {
            if(ans)
                answer.insert({linesitr->first,linesitr->second});
        }
        else
        {
            if(!ans)
                answer.insert({linesitr->first,linesitr->second});
        }
    }

    if(debug)
        cout<<"\n****/ans/***\n";

    //answers shown
    for(ansiterator=answer.begin(); ansiterator!=answer.end(); ansiterator++)
    {
        if(linenumprint)
            cout<<ansiterator->first<<" "<<ansiterator->second<<"\n";
        else
            cout<<ansiterator->second<<"\n";
    }

    if(debug)
        cout<<"****/ans/***\n\n";

    return true;
}


bool isblankorextra(char a)
{
    if(a==' '||a==','||a=='.'||a=='?'||a=='!')
        return true;
    return false;
}

//illegal case
void errormessage(string message)
{
    cout<<"Illegal symbol "<<message<<"\n";
}

//case handler
bool casehandler(string commands)
{
    bool Err=false;
    //i=0 have '-'
    for(int i=1; i<commands.length(); i++)
    {
        switch(commands[i])
        {
        case 'i':
            if(ignorecases!=true)
                ignorecases=true;
            else
            {
                errormessage("i");
                return Err;
            }
            break;

        case 'w':
            if(addblankswhilecheck!=true)
                addblankswhilecheck=true;
            else
            {
                errormessage("w");
                return Err;
            }
            break;

        case 'v':
            if(invert!=true)
                invert=true;
            else
            {
                errormessage("v");
                return Err;
            }
            break;

        case 'n':
            if(linenumprint!=true)
                linenumprint=true;
            else
            {
                errormessage("n");
                return Err;
            }
            break;
        default:
            errormessage(commands[i]+"");
        }
    }
    return true;
}
//breaking command into pieces
vector<string> breakcommand(string command, int *isCorrectCommand)
{
    int pos=0;
    vector<string>ans;
    /*
    while(command.find(" ")!=string::npos)
    {
        pos=command.find(" ");
        ans.push_back(command.substr(0,pos));
        if(debug)
            cout<<command.substr(0,command.find(" "))<<" ";
        command.erase(0,pos+1);
    }
    ans.push_back(command);

    if(debug)
        cout<<command<<" ";
    */
    string specialcommand=command.substr(0,command.find(" "));
    if(specialcommand.find('-')!=string::npos)
    {
        ans.push_back(specialcommand);
        //illegal error case
        if(command.find(" ")!=string::npos)
			command.erase(0,command.find(" ")+1);
    }
	else{
		//- not present
		ans.push_back("-");
	}

    string file_name=command.substr(command.find_last_of(" ")+1);
    
	//illegal case check
    if(command.find(" ")!=string::npos)
		command.erase(command.find_last_of(" "));
    
	if(command[0]=='\"' && command[command.size()-1]=='\"')
		ans.push_back(command.substr(1,command.size()-2));
	else{
		if(command.find(" ")!=string::npos)
		{	errormessage(command);
			*isCorrectCommand=0;
		}
		ans.push_back(command);
    }
	ans.push_back(file_name);
	if(debug)
    {
        cout<<" "<<ans[0]<<"*";
        cout<<" "<<ans[1]<<"*";
        cout<<" "<<ans[2]<<"*"<<"\n";
    }
    
    return ans;
}

// For reading lines from sample .txt
map<int,string> readline(string file)
{
    map<int,string>a;
    fstream newfile;
    int i=1;
    newfile.open(file,ios::in);
    if (newfile.is_open())
    {
        string tp;
        while(getline(newfile, tp))  //read data from file object and put it into string.
        {
            a.insert({i++,tp});
        }
        newfile.close();
    }
    return a;
}

//For KMP algo
bool KMP(string text, string pattern)
{
    prefix(pattern);
    int i=0;
    int j=0;
    if(addblankswhilecheck)
        j=1;
    bool found=0;
    bool isfirstlast=0;
    while(i<text.length())
    {
        if(j==0||j==pattern.length()-1)
            isfirstlast=1;
        else
            isfirstlast=0;
        char a1=text[i];
        char b1=pattern[j];
        if(compare(a1,b1, isfirstlast))
        {
            i++;
            j++;
            if(j==pattern.length()||(i==text.length()-1&&j==pattern.length()-2))
            {
                found=1;
                break;
            }
        }
        else if(j>0)
        {
            j=F[j-1];
        }
        else if(j==0)
        {
            i++;
        }
    }
    return found;
}



//for creating prefix table KMP
void prefix(string pattern)
{
    int i=1,j=0;
    F[0]=0;
    while(i<pattern.length())
    {
        if(compare(pattern[i],pattern[j],0))
        {
            i++;
            j++;
            F[i]=F[i-1]++;
        }
        else if(j>0)
            j=F[j-1];
        else if(j==0)
        {
            F[i]=0;
            i++;
        }
    }
}

bool compare(char a, char b,bool isfirstlast)
{
    //checkwhen -w case last and first " " character cmpariosn with . , ?
    if(isfirstlast&&addblankswhilecheck)
    {
        if(isblankorextra(a)==isblankorextra(b))
            return true;
        else
            return false;
    }


    if(!ignorecases)
        return a==b;
    //ignore cases
    else
    {
        char a1=(char)0;
        if(64<a&&a<91)
            a1=a+32;
        if(96<a&&a<123)
            a1=a-32;

        if(a1==b||a==b)
            return true;
        else
            return false;
    }

}
