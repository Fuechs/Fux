import argparse
import math
import sys

def get_progress_bar(numchars, fraction=None, percent=None):
    if percent is not None:
        fraction = percent / 100.0

    if fraction >= 1.0:
        return "█" * numchars

    blocks = [" ", "▏", "▎", "▍", "▌", "▋", "▊", "▉", "█"]
    length_in_chars = fraction * numchars
    n_full = int(length_in_chars)
    i_partial = int(8 * (length_in_chars - n_full))
    n_empty = max(numchars - n_full - 1, 0)
    return ("█" * n_full) + blocks[i_partial] + (" " * n_empty)    

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--stepno", type=int, required=True)
    parser.add_argument("--nsteps", type=int, required=True)
    parser.add_argument("remainder", nargs=argparse.REMAINDER)
    args = parser.parse_args()

    nchars = int(math.log(args.nsteps, 10)) + 1
    fmt_str = "\r[{:Xd}/{:Xd}]({:6.2f}%) ".replace("X", str(nchars))
    progress = 100 * args.stepno / args.nsteps
    sys.stdout.write(fmt_str.format(args.stepno, args.nsteps, progress))
    sys.stdout.write(get_progress_bar(50, percent=progress))
    remainder_str = " ".join(args.remainder)
    sys.stdout.write(f" {remainder_str}"+' '*(50-len(remainder_str)))
    if args.stepno == args.nsteps:
        sys.stdout.write("\n")