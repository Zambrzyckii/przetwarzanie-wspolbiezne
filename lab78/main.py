import hashlib
import itertools
import multiprocessing
import sys
import time


def worker(args):
    chunk, target, charset, length = args
    for first in chunk:
        for rest in itertools.product(charset, repeat=length - 1):
            candidate = first + ''.join(rest)
            temp = hashlib.sha256(candidate.encode()).hexdigest()
            if temp == target:
                return candidate

    return None

def main():
    charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()"
    target = sys.argv[1] #"d17bf024d6ee0527ced15648cedf2e5c710a2fc2b24aec5c7549159a14c7732d"
    #target = hashlib.sha256(target.encode()).hexdigest()
    length = int(sys.argv[2])
    workers_count = 16
    chunks = []
    k,m = divmod(len(charset), workers_count)
    for i in range(workers_count):
        start = i * k + min(i,m)
        end = (i + 1) * k + min(i + 1,m)
        chunks.append(charset[start:end])

    tasks = [(c, target, charset, length) for c in chunks]

    with multiprocessing.Pool(workers_count) as p:
        t = time.time()
        for results in p.imap_unordered(worker, tasks):
            if results:
                print(results)
                print(f"{time.time() - t:.2f}")
                p.terminate()
                break

def base():

    charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()"
    target = "d17bf024d6ee0527ced15648cedf2e5c710a2fc2b24aec5c7549159a14c7732d"
    length = 4
    t = time.time()
    for candidate in itertools.product(charset, repeat=length):
        password = ''.join(candidate)
        tem = hashlib.sha256(password.encode()).hexdigest()
        if tem == target:
            print(password)
            print(time.time() - t)
            break
if __name__ == '__main__':
    main()
    base()
