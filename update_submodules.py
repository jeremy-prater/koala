import os
import subprocess

curdir = os.getcwd()

repoList = [
    ("projects", "master"),
    ("corrade", "v2020.06"),
    ("magnum", "v2020.06"),
    ("magnum-plugins", "v2020.06"),
    ("magnum-bootstrap", "master"),
    ("external/cppzmq", "v4.7.1"),
    ("external/libzmq", "v4.3.3"),
    ("external/libpng", "v1.6.37"),
    ("tools/vcpkg", "2020.11")
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
    git(repoDir, ["fetch", "origin"])
    git(repoDir, ["checkout", repo[1]])
    git(repoDir, ["pull"])
