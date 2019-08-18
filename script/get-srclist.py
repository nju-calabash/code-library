#!/usr/bin/env python3
import oyaml as yaml
recipe = yaml.load(open('recipe.yml').read(), Loader=yaml.SafeLoader)
for cat, ls in recipe.items() :
    for fn in ls :
        print('src/' + cat + '/' + fn)
