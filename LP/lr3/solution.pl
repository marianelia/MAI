% получение элемента списка
get_element_by_index([Element|_],Element,0):- !.
get_element_by_index([_|T],X,Index):- N is Index - 1, get_element_by_index(T,X,N).

% проверка корректности расположения элементов(true - если порядок неправильный и нужно менять элементы)
check_correct(List,First_idx,Second_idx):-
    get_element_by_index(List,X,First_idx),
    get_element_by_index(List,Y,Second_idx),
    X == black, Y == white, !.

% поменять местами два элемента
swap(List,First_element,Second_element,Result):-
    length(List,Len1), 
    Len2 = Len1,
    length(Result,Len2),    %создание списка Result длины Len2
    length(FHead,First_element),
    append(FHead,[FNext|FTail],List),
    append(FHead,[SNext|FTail],Tmp),
    length(SHead,Second_element),
    append(SHead,[SNext|STail],Tmp), 
    append(SHead,[FNext|STail],Result),!.

% переход между состояниями
move([H|_],Res):- move(H,Res).

move(st(List),st(Result)):-
    length(List,Len),
    Len1 is Len - 1,
    between(0,Len1,A),
    B is A + 1, B \= Len,
    check_correct(List,A,B),
    swap(List,A,B,X),
    Result = X.

% продление пути без зацикливания
% (list with st, list with st + 1 st)
prolong([X|T],[Y,X|T]):- move(X,Y), not(member(Y,[X|T])).

inverse_print([]).
inverse_print([A|T]):-inverse_print(T), write(A), nl.

int(1).
int(X):- int(Y), X is Y + 1.

% поиск в глубину
dfs(List, Result):-
	get_time(Begin),
	dfs_search([List], Result, Ans),
	inverse_print(Ans),
	get_time(End), nl,
	T is End - Begin,
	write('Time is '), write(T), nl, nl,!.

% алгоритм поиска в глубину
dfs_search([X|T],X,[X|T]).
dfs_search(P,F,L):- prolong(P,P1), dfs_search(P1,F,L).

% поиск в ширину
bfs(List, Result):-
    get_time(Begin),
    bfs_search([[List]],Result,Ans),
    inverse_print(Ans),
    get_time(End),nl,
    T is End - Begin,
    write('Time is '), write(T), nl, nl,!.

% алгоритм поиска в ширину
bfs_search([[End|T]|_], End, [End|T]).
bfs_search([Next|B], End, Ans):- findall(Z,prolong(Next,Z),T), append(B,T,Bn), bfs_search(Bn,End,Ans).
bfs_search([_|T], End, Ans):- bfs_search(T, End, Ans).

% поиск с итерационным заглублением
iddfs(Start,Finish):-
    get_time(ITER),
    int(Depth_limit),
    id_depth([Start],Finish,Path,Depth_limit),
    inverse_print(Path),
    get_time(ITER1),nl,
    T is ITER1 - ITER,
    write('Time is '), write(T), nl, nl,!.

id_depth([Finish|T],Finish,[Finish|T],0).
id_depth(Path,Finish,R,N):-  N > 0,
    prolong(Path,New_path), N1 is N - 1,
    id_depth(New_path,Finish,R,N1).

% поиск с итерационным заглублением с нахождением кратчайшего пути первым
iddfs1(Start,Finish,Path):-
    MaxLevel = 20,
    int(Level),
    (Level > MaxLevel, !;
        id_depth([Start],Finish,Path,Level)).
