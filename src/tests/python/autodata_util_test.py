
import autodata_util as Util
import Poco_Dynamic as Dyn

var = Dyn.Var( 7.7 );
print( "Convert to int: {}".format( Util.ConvertInt( var ) ) )
print( "Convert to dbl: {}".format( Util.ConvertDouble( var ) ) )
print( "Convert to str: {}".format( Util.ConvertString( var ) ) )
