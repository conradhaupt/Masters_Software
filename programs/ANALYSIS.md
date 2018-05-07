# Analysis of existing programs

## galtay/hilbert_curve

 Based on [1] algorithm but built in Python. [1] uses gray-code that over compensates for the Hilbert pattern which is then compensated for by reworking parts.

## google/hilbert

Loops down through each order/level of the Hilbert curve mapping a single value at a time.

## cortesi/scurve

### hilbert

Same as galtay/hilbert_curve [1].

### graycurve

Modifies each bit, bit-by-bit.

### zigzag

Back-and-forth snaking row-major order.

### zorder

Morton Order.

## arthursw/space-filling-curves

Iterative recursive function in JS.

## cne1x/sfseize

Uses Compact Hilbert Curve algorithm from [2].

## aditi137/Hilbert

Same as [1].

## dholzmueller/sfcpp

Uses predefined mappings in large arrays to store transformations from one depth level to another and between coordinates.

## References

[1] J. Skilling, ‘Programming the Hilbert curve’, AIP Conference Proceedings, vol. 707, no. 1, pp. 381–387, Apr. 2004.

[2] C. H. Hamilton and A. Rau-Chaplin, ‘Compact Hilbert Indices for Multi-Dimensional Data’, in First International Conference on Complex, Intelligent and Software Intensive Systems, 2007. CISIS 2007, 2007, pp. 139–146.
