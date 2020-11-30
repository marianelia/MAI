% Task 2: Relational Data

% The line below imports the data
:- encoding(utf8).
:- ['three.pl'].

% сумма элементов в списке
sum([],0). 
sum([grade(_,Mark)|T],Sum):-sum(T,RSum), Sum is RSum + Mark.

%длина списка
lenght([],0).
lenght([_|T],L):-lenght(T,L_T), L is L_T+1.

% нахождение мвксимального числа из списка
max([],0).
max([H|T],Max_num):-max(T,Tmp_num),H >=Tmp_num, Max_num is H.
max([H|T],Max_num):-max(T,Tmp_num),H =< Tmp_num, Max_num is Tmp_num.

% задание 1 - Для каждого студента, найти средний балл (студент, средняя оценка)
average_marks_for_stud(Student, Average_marks):-student(_,Student,Marks_list),
    sum(Marks_list,Sum),lenght(Marks_list,Len),Average_marks is Sum/Len.

%Сдал ли студент экзамены. True в случае успешной сдачи всех экзаменов
successfull_exams(Student):-student(_,Student,Marks_list),not(member(grade(_,2),Marks_list)).

% задание 2 - Для каждого предмета, найти количество не сдавших студентов
count_stud_for_subjects(Subject,Count_stud):- subject(Abb_subj,Subject),
    findall(Marks_list,(student(_,_,Marks_list),list_grade(Marks_list,Abb_subj)),All_marks),length(All_marks,Count_stud).

list_grade([grade(Abb_subj,2)|_],Abb_subj).
list_grade([_|T],Abb_subj):-list_grade(T,Abb_subj).

%задание 3 -  Для каждой группы, найти студента (студентов) с максимальным средним баллом
average_stud_for_group(Group,Student):-
  findall(Av_mark,(student(Group,Stud,_),average_marks_for_stud(Stud,Av_mark)),All_av_marks),max(All_av_marks, Max_av_mark),
  findall(St,(student(Group,St,_),average_marks_for_stud(St,Tmp),Tmp == Max_av_mark),Student),!.
