
import std

ipair = std.PairInt( 1, 2 )
print( "PairInt: {0}, {1}".format( ipair.first, ipair.second ) )

fset = std.SetFloat( [ 4, 3 ] )
fset.add( 6 )
fset.add( 5 )
print( "SetFloat: [{0}]".format( ', '.join( map( str, fset ) ) ) )

dvec = std.VectorDouble( [ 7, 8 ] )
dvec.append( 9 )
dvec.append( 10 )
print( "VectorDouble: [{0}]".format( ', '.join( map( str, dvec ) ) ) )
