# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    leaks2.sh                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iel-amra <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/29 10:26:48 by iel-amra          #+#    #+#              #
#    Updated: 2021/12/01 13:06:51 by iel-amra         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

cp config.txt src
cp ../* tempo 2>/dev/null
for F in $(ls tempo)
	do sed -i '' '1i\
		void	*ft_malloc(size_t size);
	' tempo/$F
done
for F in $(ls tempo)
	do sed -i '' '1i\
		#include <stdlib.h>
	' tempo/$F
done

sed -i '' -e 's/\([^a-zA-Z_]\)malloc(/\1ft_malloc(/g' tempo/*.c
cp src/* tempo
sed -i '' -e 's/\([^ 	]*\)\./tempo\/\1./g' tempo/config.txt
#sed -i '' -e 's/\([^ 	]*\)\.h /tempo\/\1.h /g' tempo/config.txt
echo 'Test des fuites memoires :'
$(sed -n '2p' tempo/config.txt)
mv a.out tempo
for I in $(sed -n '1p' tempo/config.txt)
do echo $I > tempo/max.txt
	echo "Le malloc numero $I echoue"
	$(sed -n '3p' tempo/config.txt)
done
echo 'Test des leaks :'
$(sed -n '4p' tempo/config.txt)
mv a.out tempo
for I in $(sed -n '1p' tempo/config.txt)
do echo $I > tempo/max.txt
	echo "Le malloc numero $I echoue"
	$(sed -n '5p' tempo/config.txt) > log.txt
	grep 'leaked' log.txt
done
rm log.txt
rm tempo/*
