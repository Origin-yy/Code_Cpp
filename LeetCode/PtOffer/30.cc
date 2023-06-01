// https://leetcode.cn/problems/bao-han-minhan-shu-de-zhan-lcof/?envType=study-plan&id=lcof&plan=lcof&plan_progress=x5y85v3j
class MinStack {
    stack<int> s1, s2;
public:
    /** initialize your data structure here. */
    MinStack() {
        
    }
    
    void push(int x) {
        s1.push(x);
        if(s2.empty() || x <= s2.top()){
            s2.push(x);
        }
    }
    
    void pop() {
        if(s1.top() == s2.top()){
            s1.pop();
            s2.pop();
        }else{
            s1.pop();
        }
    }
    
    int top() {
        return s1.top();
    }
    
    int min() {
        return s2.top();
    }
};
