# № Отчет по лабораторной работе №3
## по курсу "Логическое программирование"

## Решение задач методом поиска в пространстве состояний

### студент: Лагуткина М. С.

## Результат проверки

| Преподаватель     | Дата         |  Оценка       |
|-------------------|--------------|---------------|
| Сошников Д.В. |              |               |
| Левинская М.А.|              |     5         |

> *Комментарии проверяющих (обратите внимание, что более подробные комментарии возможны непосредственно в репозитории по тексту программы)*


## Введение

Пространство состояний - это набор ситуаций. Совершая какие-то действия можно перейти из одного состояния в другое. Пространство состояний возникает при разбиении решения задачи на отдельные шаги. Здесь удобно рассматривать два состояния: начальное, когда гипотеза еще не выбрана, и конечное, когда гипотеза найдена и она будет являться допустимым решением задачи. В процессе поиска гипотезы есть операции перехода в следующее состояние.

Если представить пространство состояний как граф, в вершинах которого находятся состояния, то путь от начальной вершины до конечной будет показывать набор состояний, являющийся решением задачи. То есть такие задачи сводятся к поиску по графу. В этой лабораторной работе были рассмотрены такие алгоритмы поиска, как поиск в глубину, в ширину, а также в глубину с итеративным погружением.

Обычно для представления связей в графе в программировании используется матрица смежности. Но в Прологе можно задать граф при помощи перечисления дуг, связывающих вершины.Кроме того, Пролог позволяет задавать дуги при помощи правил, что очень удобно в больших и сложных графах.

## Задание
7. Вдоль доски расположены лунки, в каждой из которых лежит черный или белый шар. Одним ходом можно менять местами два соседних шара. Добиться того, чтобы сначала шли белые шары, а за ними - черные. Решить задачу за наименьшее число ходов.

## Принцип решения

Общий принцип решения задачи состоит в том, что мы идем по списку, рассматривая два шара, если шары лежат в неправильной последовательности, меняем их местами. Я использовала три алгоритма поиска: поиск в глубину, поиск в ширину и поиск с итеративным погружением, которые отражаются в предикатах `dfs(List, Result)`,`bfs(List, Result)`,`iddfs(Start,Finish)` соответственно.

Сначала были реализованы вспомогательные предикаты для успешной работы алгоритмов поиска. Предикат `move(List, Result)`отражает переход между состояниями в графе. Во втором правиле, проходя по списку, представляющему одно состояние, меняем элементы,которые стоят не на своем месте. Это позволяет получить другое состояние. Предикат `between(Low,Hight,Value) `генерирует целые числа от 0 до Hight, то есть в данном все числа от 0 до длины списка,затем производится проверка в предикате `check_correct(List,First_idx,Second_idx)` на то, в правильном ли порядке стоят элементы с меньшим порядковым индексом относительно элементов с большим порядковым индексом, если элементы стоян неправильно то с помощью `swap(List,First_element,Second_element,Result)` элементы меняются местами.
```prolog
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
```
Предикат `prolong(List,Long_list)` нужен, чтобы продлить все пути в графе, предотвращая зацикливания. Также реализован `inverse_print(List)`, который выводит на экран решения в обратном порядке, а также в столбик для улучшения читаемости.
```prolog
% продление пути без зацикливания
prolong([X|T],[Y,X|T]):- move(X,Y), not(member(Y,[X|T])).

inverse_print([]).
inverse_print([A|T]):-inverse_print(T), write(A), nl.
```
В предикате `dfs(List, Result)` производится поиск пути от начального состояния к конечному, с помощью обхода в глубину. В данном случае обход в глубину работает, пока возможно продление пути и не достигнута конечная вершина. Так как путь записан в обратном порядке, его необходимо выводить в обратном порядке. 
Поиск в глубину реализован с помощью вспомогательного предиката `dfs_search([X|T],X,[X|T])`.

```prolog
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
```
В предикате `bfs(List, Result)` производится поиск решения с помощью обхода в ширину. Для этого обхода используется очередь из путей, которые можно продлить. Продленные пути добавляются в конец очереди, а путь, который мы продлевали удаляется. Если первый элемент очереди - путь который ведет в конечную вершину, поиск завершается. Найденный путь гарантированно будет кратчайшим.
```prolog
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
```
`iddfs(Start,Finish)` - поиск с итеративным углублением.Здесь используется идея метода поиска в глубину, однако глубина поиска ограничивается некоторым значением, что позволяет найти кратчайший путь.
```prolog
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

```

## Результаты

```prolog
?-  dfs(st([black,black,white,black,white,black,white]),st([white,white,white,black,black,black,black])).
st([black,black,white,black,white,black,white])
st([black,white,black,black,white,black,white])
st([white,black,black,black,white,black,white])
st([white,black,black,white,black,black,white])
st([white,black,white,black,black,black,white])
st([white,white,black,black,black,black,white])
st([white,white,black,black,black,white,black])
st([white,white,black,black,white,black,black])
st([white,white,black,white,black,black,black])
st([white,white,white,black,black,black,black])

Time is 0.004999876022338867

true.
?- bfs(st([black,black,white,black,white,black,white]),st([white,white,white,black,black,black,black])).
st([black,black,white,black,white,black,white])
st([black,white,black,black,white,black,white])
st([white,black,black,black,white,black,white])
st([white,black,black,white,black,black,white])
st([white,black,white,black,black,black,white])
st([white,white,black,black,black,black,white])
st([white,white,black,black,black,white,black])
st([white,white,black,black,white,black,black])
st([white,white,black,white,black,black,black])
st([white,white,white,black,black,black,black])

Time is 0.06400394439697266

true.
?- iddfs(st([black,black,white,black,white,black,white]),st([white,white,white,black,black,black,black])).
st([black,black,white,black,white,black,white])
st([black,white,black,black,white,black,white])
st([white,black,black,black,white,black,white])
st([white,black,black,white,black,black,white])
st([white,black,white,black,black,black,white])
st([white,white,black,black,black,black,white])
st([white,white,black,black,black,white,black])
st([white,white,black,black,white,black,black])
st([white,white,black,white,black,black,black])
st([white,white,white,black,black,black,black])

Time is 0.042001962661743164

true.
```
| Алгоритм поиска |  Длина найденного первым пути  |  Время работы  |
|-----------------|:-------------------------------:|:-------------------:|
| В глубину       |            10                    |       0.005 sec       |
| В ширину        |            10                    |       0.064 sec       |
| ID              |            10                    |       0.042 sec         |

## Выводы
Нельзя не отметить, что реализовать все три алгоритма на Прологе было достаточно удобно: для алгоритмов были написаны общие предикаты, что сократило объем кода, из-за чего читаемость кода лучше, чем если бы эти алгоритмы были реализованы на другом языке программирования.

Все три алгоритма справились со своей задачей, но наиболее эффективным оказался поиск в глубину, как видно по анализу времени работы. Это не удивительно, так как решение задачи тривиально и единственно. Несложно заметить, что поиск в ширину имеет экспоненциальную сложность по времени и по памяти. Поиск в глубину экономичен по памяти, но в наивной реализации  неустойчив и имеет экспоненциальную сложность по времени. Поиск с итеративным углублением — это оптимизация поиска в глубину и в ширину, которая гарантированно позволяет найти самое близкое к начальному состоянию решение и избежать экспоненциальной сложности.

В зависимости от поставленной задачи, могут применяться разные алгоритмы. Например, если есть ограничение по памяти, то лучше использоваль поиск в глубину, если нужно найти кратчайший путь, то стоит использовать поиск в ширину. Несмотря на то, что поиск с итеративным углублением является оптимизацией, его можно применять только на простых задачах.
