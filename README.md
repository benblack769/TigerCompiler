# ReadMe
## Ben Black, Brandon Finley


### Issues Encountered: 
After outlining the initial grammar and running bison -d, we had roughly 30 reduce/reduce errors and roughly 100 shift/reduce errors. 
We solved the reduce/reduce errors in one swoop by amppending a missing terminal in our grammar. 
We eliminated a number of the shift/reduce errors by first dictating precedence for the mathematical operators. 
We eliminated more by dictating precedence for the IF ELSE keywords.
As of Monday March 19 2018 we have 60 remaining S/R errors, all having to do with our mathematical operators. 
