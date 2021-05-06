try:
    inp = open("input.txt")
    out = open("output.pl", mode = 'w')
    flag_indi = -1
    flag_famc = -1
    flag_fams = -1

    name = []
    surname = []
    fams = []
    famc = []
    sex = []

    for line in inp:
        
        if (flag_indi == 1):
            if(line.find('GIVN') != -1):
                tokens_mass = line.split()
                name.append(tokens_mass[2])

            if(line.find('SURN') != -1):
                tokens_mass = line.split()
                surname.append(tokens_mass[2])

            if(line.find('SEX') != -1):
                tokens_mass = line.split()
                sex.append(tokens_mass[2])

            if(line.find('FAMS') != -1):
                tokens_mass = line.split()
                fams.append(tokens_mass[2])
                flag_fams = 1

            if(line.find('FAMC') != -1):
                tokens_mass = line.split()
                famc.append(tokens_mass[2])
                flag_famc = 1
            
            if(line.find('RIN') !=-1):
                if(flag_famc == -1):
                    famc.append('-1')
                if(flag_fams == -1):
                    fams.append('-1')

                flag_indi = -1
                flag_famc = -1
                flag_fams = -1   
                         
        if line.find('INDI') != -1 :
            flag_indi = 1

    for i in range(0, len(famc)-1):
        for j in range(0, len(fams)-1):
            if(famc[i] == fams[j]) & ((famc[i] != '-1') | (fams[j] != '-1')):
                for k in range(j+1, len(fams)-1):
                    if (fams[k] == fams[j]) & ((fams[k] != '-1') | (fams[j] != '-1')):
                        if (sex[k] == 'M'):
                            c_name = '\'' + name[i] + ' ' + surname[i] + '\''
                            f_name = '\'' + name[k] + ' ' + surname[k] + '\''
                            m_name = '\'' + name[j] + ' ' + surname[j] + '\''
                            out.write('parents('+ c_name + ',' + f_name + ','+ m_name + ').\n')
                        else:
                            c_name = '\'' + name[i] + ' ' + surname[i] + '\''
                            f_name = '\'' + name[j] + ' ' + surname[j] + '\''
                            m_name = '\'' + name[k] + ' ' + surname[k] + '\''
                            out.write('parents('+ c_name + ',' + f_name + ','+ m_name + ').\n')  
finally:
    inp.close()
    out.close()
