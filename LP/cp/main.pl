%тут располагаются факты о родстве по дереву в виде:
parents('Maria Lagutkina','Sergey Lagutkin','Nataliia Lagutkina').


sibling_1(X,Y) :- parents(X,A,B),parents(Y,A,B), X\=Y.
sibling_2(C,D) :- parents(C,X,_),parents(D,Y,_),sibling_1(X,Y).
sibling_2(C,D) :- parents(C,_,X),parents(D,Y,_),sibling_1(X,Y).
sibling_2(C,D) :- parents(C,X,_),parents(D,_,Y),sibling_1(X,Y).
sibling_2(C,D) :- parents(C,_,X),parents(D,_,Y),sibling_1(X,Y).
sibling_3(E,F) :- parents(E,C,_),parents(F,D,_),sibling_2(C,D).
sibling_3(E,F) :- parents(E,_,C),parents(F,D,_),sibling_2(C,D).
sibling_3(E,F) :- parents(E,C,_),parents(F,_,D),sibling_2(C,D).
sibling_3(E,F) :- parents(E,_,C),parents(F,_,D),sibling_2(C,D).

sex_list('Maria Lagutkina', f).
sex_list('Natalia Namestnikova', f).
sex_list('Artem Vasiliev', m).
sex_list('Pavel Vasiliev', m).

sex(X,m):-(parents(_,X,_);sex_list(X,m)),!.
sex(X,f):-(parents(_,_,X);sex_list(X,f)),!.

father(Dad, Child):-parents(Child,Dad,_), !.
mother(Mom, Child):-parents(Child,_,Mom), !.

child(X, Parent) :- parents(X, Parent,_); parents(X,_,Parent).
son(X, Parent):-child(X,Parent), sex(X, m).
daughter(X, Parent):-child(X,Parent), sex(X, f).

sibling(X,Y) :- parents(X,A,B),parents(Y,A,B), X\=Y.
sister(Sister, X):-sibling(Sister,X), sex(X, f),!.
brother(Brother, X):- sibling(Brother,X), sex(X, m),!.

relations(father, Father, Child):- father(Father, Child).
relations(mother, Mother, Child):- mother(Mother, Child).

relations(husband, Husband, Wife):- parents(_,Husband,Wife).
relations(wife, Wife, Husband):- parents(_,Husband,Wife).

relations(brother, Brother, X):- brother(X,Brother).
relations(sister, Sister, Y):- sister(Y,Sister).
relations(sibling, Person, X):- sibling(X,Person).

relations(son, Child, Parent):- son(Child, Parent).
relations(daughter, Child, Parent):- daughter(Child, Parent).

relations(parent, Parent, Child):- parents(Child,Parent,_);parents(Child,_,Parent).
relations(child, Child, Parent):- child(Child, Parent).

%проверка допустимых отношений родства
number_of_relation(X):- member(X, [father, mother, son, daughter, brother, sister, husband, wife]).

%Задание 4

%замена на связи между людьми
name_to_relation([_],[]):-!.
name_to_relation([First, Second|T], Result_list):-
    relations(Relation, First, Second),
    Result_list = [Relation|TRes],
    name_to_relation([Second|T], TRes),!.

%поиск в ширину
move(X,Y):- relations(_, X, Y).
prolong([X|T], [Y,X|T]):- move(X, Y), not(member(Y, [X|T])).

bfs_relative(X, Y, Res):- bfs_search([[X]],Y, L), reverse(L, Res).

bfs_search([[End|T]|_], End, [End|T]).
bfs_search([Next|B], End, Ans):- findall(Z,prolong(Next,Z),T), append(B,T,Bn), bfs_search(Bn,End,Ans).
bfs_search([_|T], End, Ans):- bfs_search(T, End, Ans).

relative(X, Y, Res):- bfs_relative(X, Y, Res1), !, name_to_relation(Res1, Res).

%список проходимых узлов (имен)
relative_in_names(X, Y, Res):- number_of_relation(Res), !, relations(Res, X, Y).


% Задание 5: вопросы по степени родства

%основные составляющие части вопросов
how_or_who(X):- member(X, ['How', 'Who', how, who]).
many_or_much(X):- member(X, [much, many]).
does(X):- member(X,[do,does]).
has(X):-member(X,[has,have]).
is(X):- member(X,['Is', is]).
he_or_she(X):- member(X, [his, her, he, she]).
suffix("'s").
question_mark(?).

%пренадлежность к множественому числу
is_plural(X):-member(X, [sons, daughters, brothers, sisters]).

%приведение к единственному числу
plural_to_singular(son, sons). 
plural_to_singular(daughter, daughters).
plural_to_singular(sister, sisters).
plural_to_singular(brother, brothers).

%проверка прилагательного
is_adj(X):-member(X, [good,bad,beautiful]).
%предикаты для разбора остального предложения
shift(Y,X):- Y = X.
list1([A,B,C,D,E],Rel,Does,Name,Has,Q):- Rel = A, Does = B, Name = C, Has = D, Q = E.
list2([B,C,D,E],Does,Name,Has,Q):- Does = B, Name = C, Has = D, Q = E.
list3([D,E],Rel,Q):- Rel = D, Q = E.
list4([E],Q):- Q = E.

%Далее описаны предикаты для каждого вида вопросов

response_to_request([QW | T]):- how(QW),(response_to_request_how2(T),!;response_to_request_how1(T),!).
response_to_request([QW | T]):- who(QW),(response_to_request_who2(T),!;response_to_request_who1(T),!).
response_to_request([Is | T]):- is(Is),(response_to_request_is2(T),!;response_to_request_is1(T),!).

%[how, many,<adjective>(or nothing), <relation>, does, <person> , has , ?]
response_to_request_how1([ Qual,Adjective | T]):-
            many_or_much(Qual), 
            (is_adj(Adjective), list1(T,Relation, Does, Name1, Has, Q);
                  shift(Relation,Adjective),list2(T,Does, Name1, Has, Q)),
      is_plural(Relation), 
      does(Does), 
      (sex(Name1, m);sex(Name1, f)), has(Has), question_mark(Q),!,
      nb_setval(last_name, Name1),
      plural_to_singular(Rel, Relation),
      setof(X, relative_in_names(X, Name1, Rel),L),
      length(L, Len),!,

      write(Name1), write(" has "),
      ((Len =:= 1, write(Len), write(" "), write(Rel));(Len \= 1, write(Len), write(" "), write(Relation))),!.

%[how, many,<adjective>(or nothing), <relation>, does, he(she), has, ?]
response_to_request_how2([Qual,Adjective | T]):- 
       many_or_much(Qual), 
            (is_adj(Adjective), list1(T,Relation, Does, Name, Has, Q);
                  shift(Relation,Adjective),list2(T,Does, Name, Has, Q)),
      is_plural(Relation),
      does(Does), he_or_she(Name),
      has(Has), question_mark(Q), 
      nb_getval(last_name, Name1),
      plural_to_singular(Rel, Relation),
      setof(X, relative_in_names(X, Name1, Rel), L), 
      length(L, Len),

      write(Name1), write(" has "),
      ((Len =:= 1,write(Len),write(" "),write(Rel));(Len \= 1,write(Len),write(" "),write(Relation))),!.


%[who, is, <person>, "'s", <adjective>(or nothing), <relation>(any), ?]
response_to_request_who1([Is, Name, Suf, Adjective | T]):- 
      is(Is), (sex(Name, m);sex(Name, f)), suffix(Suf),
            (is_adj(Adjective), list3(T,Relation, Q);
                  shift(Relation,Adjective),list4(T, Q)),
      number_of_relation(Relation), question_mark(Q), !,
      nb_setval(last_name, Name),
      relations(Relation, Res_name, Name),

      write(Res_name), write(" is "), write(Name), write("'s "), write(Relation).


%[who, is, his(her),<adjective>(or nothing), <relation>, ?]
response_to_request_who2([Is, Person,Adjective | T]):- 
      is(Is), he_or_she(Person),
            (is_adj(Adjective), list3(T,Relation, Q);
                        shift(Relation,Adjective),list4(T, Q)),
      number_of_relation(Relation), question_mark(Q),!,
      nb_getval(last_name, Name),
      relations(Relation, Res_name, Name),

      write(Res_name), write(" is "), write(Name), write("'s "), write(Relation).

%[is, <person>, <person>, "`s",<adjective>(or nothing), <relation>, ?]
response_to_request_is1([Name1, Name2, Suf, Adjective | T]):- 
      nb_setval(last_name, Name2), (sex(Name1, m);sex(Name1, f)), (sex(Name2, m);sex(Name2, f)), suffix(Suf),
            (is_adj(Adjective), list3(T,Relation, Q);
                        shift(Relation,Adjective),list4(T, Q)),
      number_of_relation(Relation), question_mark(Q), 
      relations(Relation, Name1, Name2), !.

%[is, <person>, his(her), <adjective>(or nothing), <relation>, ?]
response_to_request_is2([Name, Person, Adjective | T]):- 
      (sex(Name, m);sex(Name, f)), 
      he_or_she(Person),     
            (is_adj(Adjective), list3(T,Relation, Q);
                        shift(Relation,Adjective),list4(T, Q)), 
      number_of_relation(Relation), question_mark(Q),
      nb_getval(last_name, Name1), relations(Relation, Name, Name1), !.
