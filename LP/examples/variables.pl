%Опишите предикат, который генерирует бесконечную последовательность нулей (т.е. при каждом возврате возвращает 0 как очередное решение)
int(0).
int(M) :- int(N), M is N.

%Опишите предикат, который генерирует бесконечную последовательность целых чисел 
%(т.е. сначала возвращает 1, потом - 2, и так далее до бесконечности)Иммерсивное средство чтения
int(1).
int(M) :- int(N), M is N+1.

%Опишите предикат, который генерирует бесконечную последовательность факториалов
int(1).
int(N) :- int(Prev), N is Prev + 1.
fact(1, 1) :- !.
fact(N, F) :- Prev is N - 1, fact(Prev, PF), F is N * PF. 
answer(F) :- int(I), fact(I, F).
