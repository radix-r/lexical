echo "Start main tests"
gcc lexical.c -o lex

echo " Begin lexical-1 testing (inputLex.txt)"
./lex inputLex.txt > lexical-stu1.log
diff lexical-stu1.log lexBase1.log

echo " Begin lexical-2 testing (symIn.txt) "
./lex symbIn.txt > lexical-stu2.log
diff lexical-stu2.log lexBase2.log

echo " Begin lexical-3 testing (err1In.txt)"
./lex err1In.txt > lexical-stu3.log
diff lexical-stu3.log lexBase3.log

echo " Begin lexical-4 testing (err2In.txt)"
./lex err2In.txt > lexical-stu4.log
diff lexical-stu4.log lexBase4.log
  
echo " Begin lexical-5 testing (err3In.txt)"
./lex err3In.txt > lexical-stu5.log
diff lexical-stu5.log lexBase5.log

echo " Begin lexical-6 testing (err4In.txt)"
./lex err4In.txt > lexical-stu6.log
diff lexical-stu6.log lexBase6.log

echo " Begin lexical-7 testing (trkTestIn.txt)"
./lex trkTestIn.txt > lexical-stu7.log
diff lexical-stu7.log lexBase7.log

