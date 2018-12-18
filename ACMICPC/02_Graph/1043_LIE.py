import sys
import math
from collections import deque

f_input = sys.stdin
N, M = (0, 0)
know_truth_p = []
parties = []
cache = None


def read_input():
    global N, M, know_truth_p, parties, cache
    N, M = map(int, f_input.readline().split())
    know_truth_p = [n-1 for n in map(int, f_input.readline().split())]
    know_truth_p.pop(0)

    parties = [0] * M
    for i in range(M):
        parties[i] = [n-1 for n in map(int, f_input.readline().split())]
        parties[i].pop(0)


def max_lie_party_count():
    q = deque()
    lie_party_sum = 0

    q.append((0, 0, ['_']*N))
    while len(q) > 0:
        selected, curr_party, listening = q.popleft()
        if curr_party >= M:
            lie_party_sum = max(lie_party_sum, selected)
            continue

        remain_party = M - curr_party
        if remain_party + selected <= lie_party_sum:
            continue

        next_listening = [list(listening), list(listening)]
        satisfied = [True, True]
        can_lie = True
        for p in parties[curr_party]:
            if p in know_truth_p:
                can_lie = False
                break

        for p in parties[curr_party]:
            if next_listening[0][p] == 't':
                satisfied[0] = False
            if next_listening[1][p] == 'l':
                satisfied[1] = False

            next_listening[0][p] = 'l'
            next_listening[1][p] = 't'

        if can_lie and satisfied[0]:
            q.append((selected+1, curr_party+1, next_listening[0]))

        if satisfied[1]:
            q.append((selected, curr_party+1, next_listening[1]))

    return lie_party_sum


if __name__ == "__main__":
    if len(sys.argv) > 1:
        f_input = open("input.txt", "r")

    sys.setrecursionlimit(1000)
    repeat = 1
    while repeat > 0:
        read_input()
        known_bits = 0
        ret = max_lie_party_count()
        print(ret)
        repeat -= 1
    f_input.close()
