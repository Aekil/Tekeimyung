import itertools

def printCombinaison(materialOptions):
    i = 0
    print("{ ", end = "")
    for materialOption in materialOptions:
        print("Material::eOption::" + materialOption, end = "")
        if i != len(materialOptions) - 1:
            print(", ", end = "")
        i += 1
    print(" },")

matOptions = []

with open("include/Engine/Graphics/Material.hpp") as f:
    parsingMatOptions = False;
    for line in f:
        if "#define MAT_OPTIONS" in line:
            parsingMatOptions = True
        elif parsingMatOptions:
            if "PROCESS" in line:
                matOption = line.partition("PROCESS(")[2].partition(",")[0]
                matOptions.append(matOption)
            else:
                break;

print("Material options found:", matOptions)
print("\nUnique combinaisons:\n")

for i in range(0, len(matOptions) + 1):
  for subset in itertools.combinations(matOptions, i):
    printCombinaison(subset)
