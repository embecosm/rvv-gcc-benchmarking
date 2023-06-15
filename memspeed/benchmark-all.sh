# Run all the benchmarks

# This file is part the RVV memcpy/memset evaluation

# Copyright (C) 2023 Embecosm Limited <www.embecosm.com>
# Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

# SPDX-License-Identifier: GPL-3.0-or-later

do_csv="no"
if [ $# -eq 1 ]
then
    if [ $1 = "-csv" ]
    then
	do_csv="yes"
    elif [ $1 = "--help" -o $1 = "-h" ]
    then
	printf "Usage: %s [-csv|-h|--help]\n" $0
	exit 0
    fi
fi

if [ "x${QEMU_BUILD_DIR}" = "x" ]
then
    echo "Must specify QEMU_BUILD_DIR environment variable"
    exit 1
fi

plugin=${QEMU_BUILD_DIR}/tests/plugin/libinsn.so,inline=on
sizes=$(./gensizes 19 1048576)
tmp=$(mktemp)

if [ "${do_csv}" = "yes" ]
then
    printf "%s,%s,%s,%s\n" "size" "standard/scalar" "standard/vector" \
	   "vector/vector"
else
    printf "%7s %15s %15s %15s\n" "" \
	   "standard/scalar" "standard/vector" "vector/vector"
    printf "%7s %7s %7s %7s %7s %7s %7s\n" "Size" \
	   "instr" "/byte" "instr" "/byte" "instr" "/byte"
    printf "%7s %7s %7s %7s %7s %7s %7s\n" "----" \
	   "-----" "-----" "-----" "-----" "-----" "-----"
fi

for s in ${sizes}
do
    # Standard with scalar compilation
    ss_dinst=$(qemu-riscv64 -cpu rv64,Zicsr=true -d plugin -plugin ${plugin} \
			    demo-standard-scalar ${s} dummy 2>&1 | \
		   sed -n -e 's/insns: //p')
    qemu-riscv64 -cpu rv64,Zicsr=true -d plugin -plugin ${plugin} \
		 -D ${tmp} demo-standard-scalar ${s} real
    ss_res=$?
    ss_rinst=$(sed ${tmp} -n -e 's/insns: //p')
    ss_t=$((ss_rinst - ss_dinst))
    ss_r=$((ss_t * 1000 / s))
    ss_rint=$((ss_r / 1000))
    ss_rfrac=$((ss_r % 1000))
    if [ ${ss_res} -ne 0 ]
    then
	ss_bad="${ss_bad} ${s}"
    fi

    # Standard with vector compilation
    sv_dinst=$(qemu-riscv64 -cpu rv64,v=true,vext_spec=v1.0,Zfh=true,Zicsr=true \
			    -d plugin -plugin ${plugin} \
			    demo-standard-vector ${s} dummy 2>&1 | \
		   sed -n -e 's/insns: //p')
    qemu-riscv64 -cpu rv64,v=true,vext_spec=v1.0,Zfh=true,Zicsr=true -d plugin \
		 -plugin ${plugin} -D ${tmp} demo-standard-vector ${s} real
    sv_res=$?
    sv_rinst=$(sed ${tmp} -n -e 's/insns: //p')
    sv_t=$((sv_rinst - sv_dinst))
    sv_r=$((sv_t * 1000 / s))
    sv_rint=$((sv_r / 1000))
    sv_rfrac=$((sv_r % 1000))
    if [ ${sv_res} -ne 0 ]
    then
	sv_bad="${ss_bad} ${s}"
    fi

    # Vector with vector compilation
    vv_dinst=$(qemu-riscv64 -cpu rv64,v=true,vext_spec=v1.0,Zfh=true,Zicsr=true -d plugin \
			    -plugin ${plugin} \
			    demo-vector-vector ${s} dummy 2>&1 | \
		   sed -n -e 's/insns: //p')
    qemu-riscv64 -cpu rv64,v=true,vext_spec=v1.0,Zfh=true,Zicsr=true -d plugin \
		 -plugin ${plugin} -D ${tmp} demo-vector-vector ${s} real
    vv_res=$?
    vv_rinst=$(sed ${tmp} -n -e 's/insns: //p')
    vv_t=$((vv_rinst - vv_dinst))
    vv_r=$((vv_t * 1000 / s))
    vv_rint=$((vv_r / 1000))
    vv_rfrac=$((vv_r % 1000))
    if [ ${vv_res} -ne 0 ]
    then
	vv_bad="${ss_bad} ${s}"
    fi

    # Summarize results
    if [ ${do_csv} = "yes" ]
    then
	printf "%u,%u,%u,%u\n" ${s} ${ss_t} ${sv_t} ${vv_t}
    else
	printf "%7u %7u %3u.%03u %7u %3u.%03u %7u %3u.%03u\n" ${s} \
	       ${ss_t} ${ss_rint} ${ss_rfrac} \
               ${sv_t} ${sv_rint} ${sv_rfrac} \
	       ${vv_t} ${vv_rint} ${vv_rfrac}
    fi
done

# Summarize errors
printf "\n"
if [ "x${ss_bad}" != "x" ]
then
    printf "Bad standard/scalar copies at sizes:${ss_bad}\n"
fi
if [ "x${sv_bad}" != "x" ]
then
    printf "Bad standard/vector copies at sizes:${sv_bad}\n"
fi
if [ "x${vv_bad}" != "x" ]
then
    printf "Bad standard/vector copies at sizes:${vv_bad}\n"
fi
rm ${tmp}
