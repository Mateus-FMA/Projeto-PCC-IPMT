#include <iostream>
#include <bits/stdc++.h>
using namespace std;
bool ltm(string a, string b, int m){
    int l=a.length();
    int n=b.length();
    return a.substr(0,min(l,m)) < b.substr(0,min(n,m));

}

bool leqm(const std::string &a, const std::string &b, int m){
    int l=a.length();
    int n=b.length();
    return a.substr(0,min(l,m)) <= b.substr(0,min(n,m));
}

int succ1(const std::string &txt,std::vector<int> sa, const std::string &pat){

   int answer=0;
   int n = txt.length();
   int m = pat.length();
   //cout<<"about to do leqm"<<endl;
    if (ltm(txt.substr(sa[n-1]), pat, m)){
        answer=n;
    }else if (leqm(pat, txt.substr(sa[0]), m)){
        answer= 0;

    }else{
        int l=0;
        int r=n-1;
        while ((r-l)>1){
            int h = (l+r)/2;
            if (leqm(pat, txt.substr(sa[h]), m)){
                r = h;
            }else{
                l = h;
            }
        }
        answer= r;
    }
    //cout<<"finished leqm"<<endl;
    return answer;
}
int pred1(const std::string &txt,std::vector<int> sa,const std::string &pat){
    int n = txt.length();
    int m = pat.length();
    int answer=0;
    if (ltm(txt.substr(sa[0]),pat, m)){
        answer=-1;
    }
    else if (leqm(txt.substr(sa[n-1]), pat, m)){
        answer=n-1;
    }
    else{
        int l=0, r = n-1;
        while ((r-l)>1){
            int h = (l+r)/2;
            if (leqm(txt.substr(sa[h]), pat, m)){
                l = h;
            }
            else{
                r = h;
            }
        }
        answer= l;
    }
    return answer;
}
std::vector<int> sorti (std::vector<string> suff,std::vector<int> index){
    int n=suff[0].length();
    for(int i=0;i<n-1;i++){
        if(suff[i]>suff[i+1]){
            string temp1=suff[i];
            int temp2=index[i];
            suff[i]=suff[i+1];
            suff[i+1]=temp1;
            index[i]=index[i+1];
            index[i+1]=temp2;
            i=max(i-2,-1);
        }

    }

    return index;

}
std::vector<int> build_sarr_naive(const std::string &txt){
    int n = txt.length();
    std::vector<string> suff(n);
    std::vector <int> index(n);
    for(int i=0;i<n;i++){
        //cout<<i;
        suff[i]=txt.substr(i);
        index[i]=i;
    }
    //print suff;

    index=sorti(suff,index);
     for(int i=0;i<n;i++){
        cout<<index[i]<<endl;
     }
    //print suff;
    //cout<<"sorti"<<endl;

    return index;
}
int sufArray()
{
    std::string txt = "baobab";

    std::vector<int> sa = build_sarr_naive(txt);
    //print sa;
    std::string pat= "ba";
    cout<<"here"<<endl;

    int l = succ1(txt, sa, pat);
    cout<<"succ1";
    //print "succ=",L
    int r = pred1(txt, sa, pat);
    cout<<l<<""<<r<<endl;
    //print "pred=", R
    std::vector<int> occ(l-r);
    for (int k=r+1;k<l+1;k++){
        occ[k-r-1]=sa[k];
    }
    //occ.sort();
    //print occ;
    //cout<<"sequence:"<<endl;

    
    return 0;
}


