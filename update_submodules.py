import os
import subprocess

curdir = os.getcwd()

repoList = [
    ("projects", "master"),
    ("corrade", "master"),
    ("magnum", "master"),
    ("magnum-bootstrap", "master"),
    ("magnum-plugins", "master"),
    ("external/cppzmq", "master"),
    ("external/libzmq", "v4.3.0")
]


def git(dir, args):
    args = ["git"] + args
    subprocess.run(args=args, cwd=dir)


git(".", ["submodule", "init"])
git(".", ["submodule", "update"])
git(".", ["submodule", "sync"])

for repo in repoList:
    print ("Updating Repo ==> [{}] to [{}]".format(repo[0], repo[1]))
    repoDir = curdir + "/" + repo[0]
    git(repoDir, ["reset", "--hard", "origin/HEAD"])
    git(repoDir, ["fetch", "--all"])
    git(repoDir, ["checkout", repo[1]])
