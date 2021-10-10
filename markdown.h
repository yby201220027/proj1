#ifndef MARKDOWN_H
#define MARKDOWN_H

#include <iostream>
#include <cstring>
#include <fstream>
#include<QFile>
#include<QDebug>
//#include<Qstring>

using namespace std;
struct Node
{
    int flag = 0;
    int type = 0;
    string content;
    string address;
    Node* next;
};
const std::string front[] =
{
    "", "<h1>", "<h2>", "<h3>", "<h4>", "<h5>", "<h6>","<pre><code>", "<ul>", "<li>", "<code>", "<strong>","<em>", "","", "<ol>","<blockquote>", "<p>"
};
const std::string back[] =
{
    "", "</h1>", "</h2>","</h3>", "</h4>", "</h5>", "</h6>", "</code></pre>", "</ul>", "</li>", "</code>", "</strong>","</em>", "","", "</ol>","</blockquote>", "</p>"
};
class markdown
{
public:
    markdown(const QString& filename)
    {
        Filename=filename;
    }
    void operate()
    {
        Node*head=NULL;
        Node*tail=NULL;
        int blockflag = 0;
        QFile file(Filename);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug()<<"Can't open the file!"<<endl;
        }
        while(!file.atEnd())
        {
            int start=0;
            QByteArray line = file.readLine();
            QString Str(line);
            std::string m = Str.toStdString();
            const char* str = m.c_str();
            if (strncmp(str, "    ", 4) == 0)
            {
                while (str[start])
                {
                    if (str[start] == ' ')
                        start++;
                    else
                        break;
                }
                Node* p = new Node;
                p->type = 18;
                for (int i = start; str[i] != '\0'; i++)
                    p->content += str[i];
                p->next = NULL;
                if (head == NULL)
                    head = p;
                else
                    tail->next = p;
                tail = p;
                continue;
            }
            while (str[start])
            {
                if (str[start] == ' ')
                    start++;
                else
                    break;
            }
            if (blockflag == 0 && str[0] == '\n')
            {
                Node* p = new Node;
                p->type = 19;
                p->next = NULL;
                if (head == NULL)
                    head = p;
                else
                    tail->next = p;
                tail = p;
                continue;
            }
            if (str[start] == '<')
            {
                Node* p = new Node;
                p->type = 20;
                p->content+=str;
                p->next = NULL;
                if (head == NULL)
                    head = p;
                else
                    tail->next = p;
                tail = p;
                continue;
            }
            if (strncmp(str, "```", 3) == 0 || strncmp(str, "~~~", 3) == 0)
            {
                int block = start + 3;
                Node* p = new Node;
                p->type = 7;
                p->next = NULL;
                if (head == NULL)
                    head = p;
                else
                    tail->next = p;
                tail = p;
                blockflag = !blockflag;
                continue;
            }
            if (blockflag == 1)
            {
                Node* p = new Node;
                p->type = 0;
                p->next = NULL;
                p->content += str;
                tail->next = p;
                tail = p;
                continue;
            }
            int title = start;
            int titleNum = 0;
            while (str[title] == '#')
            {
                titleNum++;
                title++;
            }
            while (str[title] == '=')
            {
                titleNum++;
                title++;
            }
            if (str[title] == ' ' && titleNum > 0 && titleNum <= 6)
            {
                title++;
                Node* p = new Node;
                p->type = titleNum;
                p->next = NULL;
                if (head == NULL)
                    head = p;
                else
                    tail->next = p;
                tail = p;
                for (int i = title; str[i] != '\n'; i++)
                    tail->content += str[i];
                continue;
            }
            int unlist = start;
            if (str[unlist]=='+'|| str[unlist] == '-'|| str[unlist] == '*')
                if (str[unlist + 1] == ' ')
                {
                    unlist += 2;
                    if (tail == NULL || tail->type != 9)
                    {
                        Node* p = new Node;
                        p->type = 8;
                        p->next = NULL;
                        if (head == NULL)
                            head = p;
                        else
                            tail->next = p;
                        tail = p;
                    }
                    Node* p = new Node;
                    if (unlist != 2)
                        p->flag = 1;
                    p->type = 9;
                    p->next = NULL;
                    if (head == NULL)
                        head = p;
                    else
                        tail->next = p;
                    tail = p;
                    for (int i = unlist; str[i] != '\n'; i++)
                        tail->content += str[i];
                    continue;
                }
            int list = start;
            while (str[list] >= '0' && str[list] <= '9')
                list++;
            if (str[list] == '.'&& str[list+1]==' ')
            {
                list+=2;
                if (tail == NULL || tail->type != 9)
                {
                    Node* p = new Node;
                    p->type = 15;
                    p->next = NULL;
                    if (head == NULL)
                        head = p;
                    else
                        tail->next = p;
                    tail = p;
                }
                Node* p = new Node;
                if (start != 0)
                    p->flag = 1;
                p->type = 9;
                p->next = NULL;
                if (head == NULL)
                    head = p;
                else
                    tail->next = p;
                tail = p;
                for (int i = list; str[i] != '\n'; i++)
                    tail->content += str[i];
                continue;
            }
            int  quote = start;
            if (strncmp(str, "> ", 2) == 0)
            {
                quote += 2;
                Node* p = new Node;
                p->type = 16;
                p->next = NULL;
                if (head == NULL)
                    head = p;
                else
                    tail->next = p;
                tail = p;
                for (int i = quote; str[i] != '\n'; i++)
                    tail->content += str[i];
                continue;
            }
            Node* p = new Node;
            p->type = 17;
            p->next = NULL;
            if (head == NULL)
                head = p;
            else
                tail->next = p;
            tail = p;
            for (int i = start; str[i] != '\n'; i++)
                tail->content += str[i];
        }
        file.close();
        content += "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n<title>markdown</title>\n</head>\n<body>\n\n";
        if(head!=NULL)
            transform(head);
        content += "\n";
        content += "</body>\n</html>\n";
        QFile file2("md.html");
        if (file2.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file2);
            out<<QString::fromStdString(content);
            out << endl;
        }
        file2.close();
    }
    void read(string str)
    {
        Node* head = NULL;
        Node* tail = NULL;
        for (int i = 0; i <= str.length(); i++)
        {
            if (str[i] == '`')
            {
                Node* p = new Node;
                p->type = 10;
                i++;
                while (str[i] != '`')
                {
                    p->content += str[i];
                    i++;
                }
                p->next = NULL;
                if (head == NULL)
                    head = p;
                else
                    tail->next = p;
                tail = p;
                continue;
            }
            if (i + 1 <= str.length() && str[i] == '*' && str[i + 1] == '*')
            {
                i++;
                Node* p = new Node;
                p->type = 11;
                i++;
                while (str[i] != '*')
                {
                    p->content += str[i];
                    i++;
                }
                i++;
                p->next = NULL;
                if (head == NULL)
                    head = p;
                else
                    tail->next = p;
                tail = p;
                continue;
            }
            if (str[i] == '*')
            {
                Node* p = new Node;
                p->type = 12;
                i++;
                while (str[i] != '*')
                {
                    p->content += str[i];
                    i++;
                }
                p->next = NULL;
                if (head == NULL)
                    head = p;
                else
                    tail->next = p;
                tail = p;
                continue;
            }
            if (str[i] == '[')
            {

                Node* p = new Node;
                p->type = 13;
                p->next = NULL;
                if (head == NULL)
                    head = p;
                else
                    tail->next = p;
                tail = p;
                ++i;
                for (; str[i] != '\n' && str[i] != ']'; ++i)
                {
                    p->content += str[i];
                }
                i += 2;
                for (; str[i] != '\n' && str[i] != ')'; ++i)
                    p->address += str[i];
                Node* q = new Node;
                q->type = 0;
                q->next = NULL;
                p->next = q;
                continue;
            }
            if (str[i] == '!' && str[i + 1] != '\n' && str[i + 1] == '[')
            {
                Node* p = new Node;
                p->type = 14;
                p->next = NULL;
                if (head == NULL)
                    head = p;
                else
                    tail->next = p;
                tail = p;
                i += 2;
                for (; str[i] != '\n' && str[i] != ']'; ++i)
                {
                    p->content += str[i];
                }
                i += 2;
                for (; str[i] != '\n' && str[i] != ')'; ++i) {
                    p->address += str[i];
                }
                Node* q = new Node;
                q->type = 0;
                q->next = NULL;
                p->next = q;
                continue;
            }
            Node* p = new Node;
            p->type = 0;
            p->next = NULL;
            if (head == NULL)
                head = p;
            else
                tail->next = p;
            tail = p;
            p->content += str[i];
        }
        if(head!=NULL)
            transform2(head);
    }
    void transform2(Node* head)
    {
        Node* p = head;
        if (p->type == 13)
        {
            content += "<a href=\"";
            content += p->address;
            content += "\">";
            content += p->content;
            content += "</a>";
        }
        else if (p->type == 14)
        {
            content += "<img alt=\"";
            content += p->content;
            content += "\" src=\"";
            content += p->address;
            content += "\" />";
        }
        else
        {
            content += front[p->type];
            content += p->content;
            content += back[p->type];
        }
        if (p->next==NULL)
            return;
        else
            transform2(p->next);
    }
    void transform(Node* head)
    {
        for (Node* p = head; p != NULL; p = p->next)
        {
            //cout << p->content;
            if (p->type == 7)
            {
                content += "<pre><code>\n";
                Node* q = p->next;
                for (; q->type == 0 ; q = q->next)
                    content += q->content;
                content += "</pre></code>\n";
                if (q->next != NULL)
                    p =q;
                else
                    break;
            }
            else if (p->type == 18)
            {
                content += "<pre><code>\n";
                Node* q = p;
                for (; q->type == 18; q = q->next)
                    content += q->content;
                content += "</pre></code>\n";
                if (q != NULL)
                {
                    p = head;
                    while (p->next != q)
                        p = p->next;
                }
                else
                    break;
            }
            else if (p->type == 8)
            {
                int flag1 = 0;
                content += "<ul>\n";
                Node* q = p->next;
                for (; q->type == 9 && q->next != NULL; q = q->next)
                {
                    if (q->flag == 1&&flag1==0)
                    {
                        content += "<ul>\n";
                        flag1 = 1;
                    }
                    content += "<li>";
                    read(q->content);
                    content += "</li>\n";
                    if (q->flag == 1 && q->next->flag==0)
                    {
                        content += "</ul>\n";
                        flag1 = 0;
                    }
                }
                if (q->next == NULL)
                {
                    content += "<li>";
                    read(q->content);
                    content += "</li>\n";
                    q = q->next;
                }
                content += "</ul>\n";
                if (q != NULL)
                {
                    p = head;
                    while (p->next != q)
                        p = p->next;
                }
                else
                    break;
            }
            else if (p->type == 15)
            {
                int flag2 = 0;
                content += "<ol>\n";
                Node* q = p->next;
                for (; q->type == 9&&q->next!=NULL; q = q->next)
                {
                    if (q->flag == 1 && flag2 == 0)
                    {
                        content += "<ol>\n";
                        flag2 = 1;
                    }
                    content += "<li>";
                    read(q->content);
                    content += "</li>\n";
                    if (q->flag == 1 && q->next->flag == 0)
                    {
                        content += "</ol>\n";
                        flag2 = 0;
                    }
                }
                if (q->next == NULL)
                {
                    content += "<li>";
                    read(q->content);
                    content += "</li>\n";
                    q = q->next;
                }
                content += "</ol>\n";
                if (q != NULL)
                {
                    p = head;
                    while (p->next != q)
                        p = p->next;
                }
                else
                    break;
            }
            else if (p->type >=1&&p->type<=6)
            {
                content += front[p->type];
                content += p->content;
                content += back[p->type];
                content += '\n';
            }
            else if (p->type == 17)
            {
                content += "<p>";
                Node* q = p;
                for (; q->type == 17 && q->next != NULL; q = q->next)
                {
                    read(q->content);
                    content += '\n';
                }
                if (q->next == NULL)
                {
                    read(q->content);
                    q = q->next;
                }
                content += "</p>\n";
                if (q != NULL)
                {
                    p = head;
                    while (p->next != q)
                        p = p->next;
                }
                else
                    break;
            }
            else if (p->type == 16)
            {
                content += "<quote>\n";
                Node* q = p;
                for (;( q->type == 16|| q->type == 17) && q->next != NULL; q = q->next)
                {
                    read(q->content);
                    content += '\n';
                }
                if (q->next == NULL)
                {
                    read(q->content);
                    content += '\n';
                    q = q->next;
                }
                content += "</quote>\n";
                if (q != NULL)
                {
                    p = head;
                    while (p->next != q)
                        p = p->next;
                }
                else
                    break;
            }
            else if (p->type == 20)
            {
                content+=p->content;
            }
        }
    }
private:
    Node* head;
    QString Filename;
    string content;
};


#endif // MARKDOWN_H
