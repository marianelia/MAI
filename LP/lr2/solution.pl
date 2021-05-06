unique_el([]).
unique_el([X|T]) :- not(member(X,T)), unique_el(T).

remove1(X,[X|T],T).
remove1(X,[Y|T],[Y|R]) :- remove1(X,T,R).

permute1([],[]).
permute1([X|T],R) :-permute1(T,L),remove1(X,R,L).

female(arkadeva). 
female(babanova).
female(karsakova).
male(dashkov).
male(ilin).
male(flerov).

human(X) :- male(X).
human(X) :- female(X).

inst(karsakova).    % Все, кроме Аркадьева и Бабанова окончили педагогический институт
inst(dashkov). 
inst(ilin).
inst(flerov). 
   
solve(Ans) :-    
    L = [person(G, geography, Age_g, Experience_g), person(E, english, Age_e, Experience_e), 
            person(F, french, Age_f, Experience_f), person(D, german, Age_d, Experience_d), 
            person(H, history, Age_h, Experience_h), person(M, math, Age_m, Experience_m)],
              
    human(G), human(E), human(F), human(D), human(H), human(M), unique_el([G,E,F,D,H,M]),

    %Преподаватель немецкого языка и преподаватель математики в студенческие годы занимались художественной гимнастикой
    female(D), female(M),

    %преп. английского работает в этом институте с тех пор, как окончил его. Преподаватели математики и истории его бывшие студенты
    inst(E), inst(M), inst(H),

    permute1([Age_g,Age_e,Age_f,Age_d,Age_h,Age_m], [1,2,3,4,5,6]),

    Age_e =:= 6,         %Преподаватель английского языка самый старший из всех по возрасту
    
    member(person(ilin,_,Age_ilin,_),L), member(person(flerov,_,Age_flerov,_),L),
    not(member(person(arkadeva,german,_,_),L)),

    Age_ilin > Age_flerov,  %Ильин старше Флерова
    Age_flerov > Age_f,        % Флеров отец преподавателя французского языка
    
    permute1([Experience_g,Experience_e,Experience_f,Experience_d,Experience_h,Experience_m],[1,2,3,4,5,6]),
    member(person(ilin,_,_,Experience_i),L),
    Experience_e =:= 6,         % Преподаватель английского языка самый старший из всех по стажу
    Experience_i < Experience_g, % стаж работы Ильина меньше, чем у преподавателя географии

    Ans = [ans_person(G, geography),ans_person(E, english),ans_person(F, french),ans_person(D, german),ans_person(H, history),ans_person(M, math)].

res_solve(List):- setof(X, solve(X), List).
