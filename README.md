## ReadMe
### Ben Black, Brandon Finley


### Issues Encountered: 
After outlining the initial grammar and running `bison -d`, we had roughly 30 reduce/reduce errors and roughly 100 shift/reduce errors.  
We solved the reduce/reduce errors in one swoop by appending a missing terminal in our grammar.  
We eliminated a number of the shift/reduce errors by first dictating precedence for the mathematical operators.  
We eliminated more by dictating precedence for the IF ELSE keywords.  
As of Monday March 19 2018 we have 60 remaining S/R errors.  
These remaining S/R errors all concern this rule:

```
expr: expr op expr
```

Upon examining the `tiger.output` file that `bison -d` produces, we found that at almost every possible instance, the compiler
is confused about whether to shift at the first `expr` or to reduce at the last `expr` present in some other rule.  
Solution pending.  

---
As of March 20 2018, we managed to resolve the S/R errors by declaring a fake token (`FAKE`) with a low precedence and applying the precedence to all rules where conflicts were present.  