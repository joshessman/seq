#include <iostream>
#include "seq.h"

using namespace seq;
using namespace seq::types;
using namespace seq::stageutil;

extern "C" bool is_cpg(char *seq, uint32_t len)
{
	return len >= 2 && seq[0] == 'C' && seq[1] == 'G';
}

static Filter& filt_cpg()
{
	return filter("is_cpg", is_cpg);
}

int main()
{
	seq::Seq s;

	/*
	 * Multiple pipelines can be added
	 * to a source sequence
	 */
	s |
	split(10,1) |
	filt_cpg() |
	print() |
	substr(6,5) |
	copy() |
	revcomp() |
	split(1,1) |
	print();

	s | split(32,32) | len() | print();

	s | print() | copy() | revcomp() | print();

	/*
	 * Vars can be used to refer back to
	 * results of previous pipelines
	 */
	Var a, b, c, d;
	a = s | print();
	b = a | substr(1,5);
	c = b | substr(1,1) | print();
	d = b | copy() | revcomp() | print();

	/*
	 * Pipelines can branch arbitrarily
	 */
	Pipeline x, y;
	x = s | split(32,1) | filt_cpg();
	x | print();
	y = x | substr(1,16);
	y | print();
	y | copy() | revcomp() | print();

	/*
	 * Global memory can be declared
	 */
	Mem m = s.mem<Int>(1000);
	Var i, v;
	i = s | split(2,1) | filt_cpg() | count();
	i | print();
	s | split(1,1) | count() | m[i];
	i | m[i];
	v = m[i];
	v | print();
	m[i] | print();

	s.source("test/data/seqs.fastq");
	s.execute(true);  // debug=true
}
