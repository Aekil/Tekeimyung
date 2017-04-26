#! /usr/bin/python3

import sys
import getopt

def use():
    print('GameUtils.py --element=system|script --name=scriptName --author=Simon AMBROISE')

def main(argv):
    element = ''
    name = ''
    author = ''
    try:
        opts, args = getopt.getopt(argv, "h", ["element=", "name=", "author="])
    except getopt.GetoptError:
        use()
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            use()
            sys.exit()
        elif opt in "--element":
            element = arg
        elif opt in "--name":
            name = arg

    if name == '':
        use()
        sys.exit(2)
    if author == '':
        author = 'Simon AMBROISE'

    if element == 'system':
        print('system')
    elif  element == 'script':
        with open('./Game/src/Game/Scripts/'+name+'.cpp', 'w+') as srcFile:
            srcFile.write("""/*
** @Author : %s
*/

#include <Engine/Components.hh>

#include <Game/Scripts/%s.hpp>

void %s::start()
{
}

void %s::update(float dt)
{
}
""" % (author, name, name, name))
            srcFile.close()
        with open('./Game/include/Game/Scripts/'+name+'.hpp', 'w+') as includeFile:
            includeFile.write("""/*
** @Author : %s
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class %s : public BaseScript
{
public:
    %s() {};
    ~%s() {};
public:
    virtual void start();
    virtual void update(float dt);
};

REGISTER_SCRIPT(%s);
""" % (author, name, name, name,name))
            includeFile.close()
    else:
        use()
        sys.exit(2)

if __name__ == "__main__":
    main(sys.argv[1:])
