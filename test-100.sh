. ./test.sh

PROG_C = "client/bin/a.out"
PROG_S = "server/bin/a.out"

# Vérifie les arguments passés au programme

./client/bin/a.out	2> $TMP >&2 || tu && fail "comp: nb arg (0) invalide".
./client/bin/a.out toto	2> $TMP >&2 || tu && fail "comp: nb arg (1) invalide".
./client/bin/a.out toto tata titi tutu	2> $TMP >&2 || tu && fail "comp: nb arg (4) invalide".
./server/bin/a.out	2> $TMP >&2 || tu && fail "comp: nb arg (0) invalide".
./server/bin/a.out toto	2> $TMP >&2 || tu && fail "comp: nb arg (1) invalide".
./server/bin/a.out tata tete titi	2> $TMP >&2 || tu && fail "comp: nb arg (3) invalide".

logs_aux
echo "ok"
exit 0
