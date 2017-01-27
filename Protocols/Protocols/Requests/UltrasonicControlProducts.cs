using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Data;
using System.Data.SqlClient;
using Protocols.Models;
using System.Text.RegularExpressions;

namespace Protocols.Requests
{
    class UltrasonicControlProducts
    {
        public static IEnumerable<IDTimeCount> IDProtocolsTable(DateTime from, DateTime to)
        {
            IList<IDTimeCount> idProtocols = new List<IDTimeCount>();

            string queryString =
           "SELECT "
           + "[IDProtocolsTable]"
           //+ " , X "
           + ", pr.ProtocolNumber"
           + ", [Date_Time], Count, op.[Operator]"

            + ", pr.[Alloy            ]"
            + ", pr.[DeliveryStatus   ]"
            + ", pr.[NormativeDocument]"
            + ", pr.[Gang             ]"
            + ", pr.[ProductCodeNumber]"
            + ", pr.[NumberPacket     ]"
            + ", pr.[Standart         ]"
            

               + " FROM("
               + " SELECT [IDProtocolsTable], [Date_Time],[IDOperator]"
                 + ", COUNT(*)OVER(PARTITION BY [IDProtocolsTable]) AS Count"
                 + ", ROW_NUMBER()OVER(PARTITION BY [IDProtocolsTable] ORDER BY [Date_Time])AS N"
               //  + " , ROW_NUMBER()OVER(PARTITION BY YEAR([Date_Time])ORDER BY [Date_Time])AS X"
               + " FROM TubesTable"
           + ")AS tmp, OperatorsTable AS op, ProtocolsTable pr"
           + " WHERE N = 1 AND tmp.[IDOperator] = op.ID AND tmp.[IDProtocolsTable] = pr.ID"
           + " AND [Date_Time] >= @_from_ AND [Date_Time] <= @_to_"
           + " ORDER BY [Date_Time], pr.[NumberPacket] DESC"
           ;
            using (SqlConnection connection = new SqlConnection(Properties.Settings.Default.StoredBaseConnectionString))
            {
                SqlCommand command = new SqlCommand(queryString, connection);

                command.Parameters.Add("@_from_", SqlDbType.DateTime);
                command.Parameters["@_from_"].Value = from;

                command.Parameters.Add("@_to_", SqlDbType.DateTime);
                command.Parameters["@_to_"].Value = to;

                command.Connection.Open();

                SqlDataReader reader = command.ExecuteReader();
                
                try
                {
                    if (reader.Read())
                    {
                        IDTimeCount t = new IDTimeCount();
                        t.ID = (int)reader[0];
                        t.NumberProtocol = (int)((reader[1] is DBNull) ? 0 : reader[1]);
                        t.TteTme = (DateTime)reader[2];
                        t.Count = (int)reader[3];
                        t.Operator = (string)reader[4];

                        t.Alloy             = (string)reader[5];
                        t.DeliveryStatus    = (string)reader[6];
                        t.NormativeDocument = (string)reader[7];
                        t.Gang              = (string)reader[8];
                        t.ProductCodeNumber = (string)reader[9];
                        t.NumberPacket      = (string)reader[10];
                        t.Standart          = (string)reader[11];
                        command.Connection.Close();
                        if (0 == t.NumberProtocol)
                        {
                            queryString =
                                "SELECT max(ProtocolNumber )AS x" 
                                + " FROM ProtocolsTable AS p"
                                + " INNER JOIN TubesTable AS t"
                                + " ON p.ID = t.IDProtocolsTable"
                                + " WHERE YEAR(t.Date_Time)= @year"
                                ;
                            command = new SqlCommand(queryString, connection);
                            command.Parameters.Add("@year", SqlDbType.Int);
                            command.Parameters["@year"].Value = t.TteTme.Year;
                            command.Connection.Open();
                            reader = command.ExecuteReader();
                           
                            reader.Read();
                            t.NumberProtocol = 1 + (int)((reader[0] is DBNull) ? 0 : reader[0]);
                            command.Connection.Close();

                            queryString = "UPDATE ProtocolsTable SET ProtocolNumber=" + t.NumberProtocol.ToString();
                            queryString += " WHERE ID=" + t.ID.ToString();

                            command = new SqlCommand(queryString, connection);
                            command.Connection.Open();
                            command.ExecuteNonQuery();
                            command.Connection.Close();

                        }

                        idProtocols.Add(t);
                    }
                }
                    catch(InvalidCastException)
                {
                   }
                finally
                {
                    reader.Close();
                }
            }
            return idProtocols;
        }

        public static IEnumerable<UltrasonicControlProductModels> HeaderProtocol(int id, long numberProtocol, DateTime dteTme, int count, string Operator)
        {
            List<UltrasonicControlProductModels> res = new List<UltrasonicControlProductModels>();

            string queryString = "SELECT [Alloy],[DeliveryStatus],[NormativeDocument],[Gang]"
              + ",[ProductCodeNumber],[NumberPacket],[Standart]"
              + " FROM [ProtocolsTable]"
              + "WHERE [ID]= @id"
            ;

            using (SqlConnection connection = new SqlConnection(Properties.Settings.Default.StoredBaseConnectionString))
            {
                SqlCommand command = new SqlCommand(queryString, connection);

                command.Parameters.Add("@id", SqlDbType.Int);
                command.Parameters["@id"].Value = id;

                command.Connection.Open();
                SqlDataReader reader = command.ExecuteReader();
                try
                {
                    if (reader.Read())
                    {
                        UltrasonicControlProductModels t = new UltrasonicControlProductModels();

                        t.Alloy = (string)reader[0];
                        t.DeliveryStatus = (string)reader[1];
                        t.NormativeDocument = (string)reader[2];
                        t.Gang = (string)reader[3];
                        t.ProductCodeNumber = (string)reader[4];
                        t.NumberPacket = (string)reader[5];
                        t.Standart = (string)reader[6];

                        t.Count = count;
                        t.TteTme = dteTme;
                        t.Operator = Operator;

                        t.NumberProtocol = numberProtocol.ToString();

                        res.Add(t);
                    }
                }
                finally
                {
                    reader.Close();
                }
            }
            return res;
        }
        private static double FindMin(int n0, int n1, int n2, int n3, double[] arr, int len)
        {
            if (len > 0)
            {
                if (n0 >= len) n0 = len - 1;
                if (n1 >= len) n1 = len - 1;
                if (n2 >= len) n2 = len - 1;
                if (n3 >= len) n3 = len - 1;
                double t = arr[n0];
                for (int i = 1 + n0; i < n1; ++i)
                {
                    if (t > arr[i]) t = arr[i];
                }
                for (int i = n3; i < n3; ++i)
                {
                    if (t > arr[i]) t = arr[i];
                }
                return t;
            }
            return 0;
        }
        private static double FindMax(int n0, int n1, int n2, int n3, double[] arr, int len)
        {
            if (len > 0)
            {
                if (n0 >= len) n0 = len - 1;
                if (n1 >= len) n1 = len - 1;
                if (n2 >= len) n2 = len - 1;
                if (n3 >= len) n3 = len - 1;
                double t = arr[n0];
                for (int i = 1 + n0; i < n1; ++i)
                {
                    if (t < arr[i]) t = arr[i];
                }
                for (int i = n3; i < n3; ++i)
                {
                    if (t < arr[i]) t = arr[i];
                }
                return t;
            }
            return 0;
        }
        public static IList<TubesPacketResult> BodyProtocol(int id)
        {
            IList<TubesPacketResult> t = new List<TubesPacketResult>();

            string queryString =
                  "SELECT t.NumberTube, s.LengthTube, s.Status"
                + ", p.N0, p.N1, p.N1, p.N3"
                + ", (SELECT TOP 1 MinMaxThickness FROM ThicknessTable WHERE ID = st.MinThicknessID)as mn"
                + ", (SELECT TOP 1 MinMaxThickness FROM ThicknessTable WHERE ID = st.MaxThicknessID)as mx"
                + " FROM ((TubesTable as t INNER JOIN StoredMeshureTable as s ON t.IDStoredMeshureTable = s.ID)"
                + " INNER JOIN StoredThicknessTable as st ON st.TubesTableID = t.ID), ProtectiveThickeningTable as p"
                + " WHERE t.IDProtocolsTable = @IDProtocols AND p.ID = st.ProtectiveThickeningTableID"
                + " ORDER BY s.ID ASC"
                ;

            using (SqlConnection connection = new SqlConnection(Properties.Settings.Default.StoredBaseConnectionString))
            {
                SqlCommand command = new SqlCommand(queryString, connection);

                command.Parameters.Add("@IDProtocols", SqlDbType.Int);
                command.Parameters["@IDProtocols"].Value = id;

                command.Connection.Open();
                SqlDataReader reader = command.ExecuteReader(CommandBehavior.SequentialAccess);
                int count;
                byte[] buffer = new byte[300];
                double[] minBuf = new double[300];
                double[] maxBuf = new double[300];
                byte[] tmpBuf = new byte[300 * sizeof(double)];

    ///*0*/		Clr<Undefined   >
	///*1*/		, Clr<DeathZone>
	///*2*/		, Clr<Nominal	>
	///*3*/	    , Clr<BorderAbove<Thickness> >
	///*4*/	    , Clr<BorderLower<Thickness> >
	///*5*/	    , Clr<BorderKlass2<Long> > 
	///*6*/	    , Clr<BorderDefect<Long> >
	///*7*/	    , Clr<BorderKlass2<Cross>>
	///*8*/	    , Clr<BorderDefect<Cross>>
	///*9*/       , Clr<BrakStrobe2<Thickness>>
    //
	///*10*/  , Clr<Cancel<Projectionist>>
	///*11*/	, Clr< Nominal, Cancel<Projectionist>>
	///*12*/	, Clr<BorderAbove<Thickness>, Cancel<Projectionist>>
	///*13*/	, Clr<BrakStrobe2<Thickness>, Cancel<Projectionist> >
	///*14*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>>
	///*15*/	, Clr<BorderDefect<Cross>, BorderAbove<Thickness>>
	///*16*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>>
	///*17*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>>
	///*18*/	, Clr<BorderDefect<Long>, BorderAbove<Thickness>>
	///*19*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>>
    //
	///*20*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
	///*21*/	, Clr<BorderDefect<Cross>, BorderDefect<Long> >
	///*22*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderAbove<Thickness>>   
	///*23*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>> 
	///*24*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
	///*25*/  , Clr<BorderKlass2<Cross>, BorderAbove<Thickness>>
	///*26*/	, Clr<BorderLower<Thickness>, BorderKlass2<Cross>>
	///*27*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>>
	///*28*/	, Clr<BorderKlass2<Long>, BorderAbove<Thickness>>
	///*29*/	, Clr<BorderLower<Thickness>, BorderKlass2<Long>>
    //
	///*30*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Long>>
	///*31*/	, Clr<BorderKlass2<Cross>, BorderKlass2<Long> >
	///*32*/  , Clr<BorderKlass2<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>   
	///*33*/  , Clr<BorderLower<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>> 
	///*34*/  , Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>>
	///*35*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long> >
	///*36*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>
	///*37*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>>    
	///*38*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
	///*39*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross> >
    //
	///*40*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderAbove<Thickness>>   
	///*41*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>>      
	///*42*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>> 
	///*43*/	, Clr<BorderLower<Thickness>, Cancel<Projectionist> >
	///*44*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*45*/	, Clr<BorderKlass2<Long> , Cancel<Projectionist>> 
	///*46*/	, Clr<BorderDefect<Long> , Cancel<Projectionist>>
	///*47*/	, Clr<BorderKlass2<Cross>, Cancel<Projectionist>>
	///*48*/	, Clr<BorderDefect<Cross>, Cancel<Projectionist>>		
	///*49*/	, Clr<BorderDefect<Cross>, BorderAbove<Thickness>, Cancel<Projectionist>>
    //
	///*50*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>, Cancel<Projectionist>>
	///*51*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*52*/	, Clr<BorderDefect<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*53*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>, Cancel<Projectionist>>
	///*54*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*55*/	, Clr<BorderDefect<Cross>, BorderDefect<Long>, Cancel<Projectionist>>
	///*56*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>   
	///*57*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, Cancel<Projectionist>> 
	///*58*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*59*/  , Clr<BorderKlass2<Cross>, BorderAbove<Thickness>, Cancel<Projectionist>>
    //
	///*60*/	, Clr<BorderLower<Thickness>, BorderKlass2<Cross>, Cancel<Projectionist>>
	///*61*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, Cancel<Projectionist>>
	///*62*/	, Clr<BorderKlass2<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*63*/	, Clr<BorderLower<Thickness>, BorderKlass2<Long>, Cancel<Projectionist>>
	///*64*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Long>, Cancel<Projectionist>>
	///*65*/	, Clr<BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>>
	///*66*/  , Clr<BorderKlass2<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>   
	///*67*/  , Clr<BorderLower<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>> 
	///*68*/  , Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>>
	///*69*/  , Clr< BorderDefect<Cross>, BorderKlass2<Long>, Cancel<Projectionist>>
    //
	///*70*/  , Clr< BorderDefect<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*71*/  , Clr< BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, Cancel<Projectionist>>    
	///*72*/  , Clr< BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*73*/  , Clr< BorderDefect<Long>, BorderKlass2<Cross>, Cancel<Projectionist>>
	///*74*/  , Clr< BorderDefect<Long>, BorderKlass2<Cross>, BorderAbove<Thickness>, Cancel<Projectionist>>   
	///*75*/  , Clr< BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, Cancel<Projectionist>>      
	///*76*/  , Clr< BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>> 
	///*77*/	, Clr<BrakStrobe2<Thickness>, BorderAbove<Thickness> >
	///*78*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness> >
	///*79*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>>
    //
	///*80*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Long> > 
	///*81*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long> >
	///*82*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>>
	///*83*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>>
	///*84*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderAbove<Thickness>>
	///*85*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderLower<Thickness>>
	///*86*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>>
	///*87*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderAbove<Thickness>>
	///*88*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderLower<Thickness>>
	///*89*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
    //
	///*90*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long> >
	///*91*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderAbove<Thickness>>   
	///*92*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>> 
	///*93*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
	///*94*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderAbove<Thickness>>
	///*95*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Cross>>
	///*96*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>>
	///*97*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Long>, BorderAbove<Thickness>>
	///*98*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Long>>
	///*99*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Long>>
    //
	///*100*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long> >
	///*101*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>   
	///*102*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>> 
	///*103*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>>
	///*104*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long> >
	///*105*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>
	///*106*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>>    
	///*107*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
	///*108*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross> >
	///*109*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderAbove<Thickness>> 
    //
	///*110*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>>      
	///*111*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>> 
	///*112*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, Cancel<Projectionist> >
	///*113*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*114*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Long> , Cancel<Projectionist>> 
	///*115*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long> , Cancel<Projectionist>>
	///*116*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, Cancel<Projectionist>>
	///*117*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, Cancel<Projectionist>>
	///*118*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*119*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderLower<Thickness>, Cancel<Projectionist>>
    //
	///*120*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*121*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*122*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderLower<Thickness>, Cancel<Projectionist>>
	///*123*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*124*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, Cancel<Projectionist>>
	///*125*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>   
	///*126*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, Cancel<Projectionist>> 
	///*127*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*128*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*129*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Cross>, Cancel<Projectionist>>
    //
	///*130*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, Cancel<Projectionist>>
	///*131*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*132*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Long>, Cancel<Projectionist>>
	///*133*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Long>, Cancel<Projectionist>>
	///*134*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>>
	///*135*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>   
	///*136*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>> 
	///*137*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>>
	///*138*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, Cancel<Projectionist>>
	///*139*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>
    //
	///*140*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, Cancel<Projectionist>>    
	///*141*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	///*142*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, Cancel<Projectionist>>
	///*143*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderAbove<Thickness>, Cancel<Projectionist>>   
	///*144*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, Cancel<Projectionist>>      
	///*145*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>> 

                var cr = new []{
                     8
                   , 15
                   , 16
                   , 17
                   , 21
                   , 22
                   , 23
                   , 24
                   , 35
                   , 36
                   , 37
                   , 38
                   , 48
                   , 49
                   , 50
                   , 51
                   , 55
                   , 56
                   , 57
                   , 58
                   , 69
                   , 70
                   , 71
                   , 72
                   , 83
                   , 84
                   , 85
                   , 86
                   , 90
                   , 91
                   , 92
                   , 93
                   , 104
                   , 105
                   , 106
                   , 107
                   , 117
                   , 118
                   , 119
                   , 120
                   , 124
                   , 125
                   , 126
                   , 127
                   , 138
                   , 139
                   , 140
                   , 141
                };

                var lo = new[] { 
                   6
                   , 18
                   , 19
                   , 20
                   , 21
                   , 22
                   , 23
                   , 24
                   , 39
                   , 40
                   , 41
                   , 42
                   , 46
                   , 52
                   , 53
                   , 54
                   , 55
                   , 56
                   , 57
                   , 58
                   , 73
                   , 74
                   , 75
                   , 76
                   , 81
                   , 87
                   , 88
                   , 89
                   , 90
                   , 91
                   , 92
                   , 93
                   , 108
                   , 109
                   , 110
                   , 111
                   , 115
                   , 121
                   , 122
                   , 123
                   , 124
                   , 125
                   , 126
                   , 127
                   , 142
                   , 143
                   , 144
                   , 145
                };

                var th = new[] {
                   4
                   , 14
                   , 16
                   , 17
                   , 19
                   , 20
                   , 23
                   , 24
                   , 26
                   , 27
                   , 29
                   , 30
                   , 33
                   , 34
                   , 37
                   , 38
                   , 41
                   , 42
                   , 43
                   , 44
                   , 50
                   , 51
                   , 53
                   , 54
                   , 57
                   , 58
                   , 60
                   , 61
                   , 63
                   , 64
                   , 67
                   , 68
                   , 71
                   , 72
                   , 75
                   , 76
                   , 78
                   , 79
                   , 85
                   , 86
                   , 88
                   , 89
                   , 92
                   , 93
                   , 95
                   , 96
                   , 98
                   , 99
                   , 102
                   , 103
                   , 106
                   , 107
                   , 110
                   , 111
                   , 112
                   , 113
                   , 119
                   , 120
                   , 122
                   , 123
                   , 126
                   , 127
                   , 129
                   , 130
                   , 132
                   , 133
                   , 136
                   , 137
                   , 140
                   , 141
                   , 144
                   , 145
                };

                var b2 = new[]{
                    9
                    , 13
                    , 77
                    , 78
                    , 79
                    , 80
                    , 81
                    , 82
                    , 83
                    , 84
                    , 85
                    , 86
                    , 87
                    , 88
                    , 89
                    , 90
                    , 91
                    , 92
                    , 93
                    , 94
                    , 95
                    , 96
                    , 97
                    , 98
                    , 99
                    , 100
                    , 101
                    , 102
                    , 103
                    , 104
                    , 105
                    , 106
                    , 107
                    , 108
                    , 109
                    , 110
                    , 111
                    , 112
                    , 113
                    , 114
                    , 115
                    , 116
                    , 117
                    , 118
                    , 119
                    , 120
                    , 121
                    , 122
                    , 123
                    , 124
                    , 125
                    , 126
                    , 127
                    , 128
                    , 129
                    , 130
                    , 131
                    , 132
                    , 133
                    , 134
                    , 135
                    , 136
                    , 137
                    , 138
                    , 139
                    , 140
                    , 141
                    , 142
                    , 143
                    , 144
                    , 145
                };

                Regex regex = new Regex(@"(.+\,.)");
                Match match;

                while (reader.Read())
                {
                    TubesPacketResult packet = new TubesPacketResult();

                    packet.NumTube = (string)reader[0];
                    packet.Length = (int)reader.GetInt32(1);
                    reader.GetBytes(2, 0, buffer, 0, 300);
                    count = packet.Length / 50;
                    string crossDef = "";
                    string longDef = "";
                    string thickDef = "";
                    string strobeDef = "";

                    byte b;

                    byte[] thick = new byte[300];
                    byte[] cross = new byte[300];
                    byte[] longA = new byte[300];
                    byte[] b2A   = new byte[300];

                    int thickCount = 0;
                    int crossCount = 0;
                    int longACount = 0;
                    int b2Count = 0;

                    for (int i = 1; i <= count; ++i)
                    {
                        b = buffer[i - 1];
                        if (cr.Contains(b))
                        {
                            cross[i - 1] = 1;
                            crossCount = i;
                        }
                        if (lo.Contains(b))
                        {
                            longA[i - 1] = 1;
                            longACount = i;
                        }
                        if (th.Contains(b))
                        {
                            thick[i - 1] = 1;
                            thickCount = i;
                        }
                        if (b2.Contains(b))
                        {
                            b2A[i - 1] = 1;
                            b2Count = i;
                        }
                    }
                    if (crossCount > 0)
                    {
                        crossDef = "Брак зоны: " + CompressString(cross, crossCount);
                    }
                    else
                    {
                        crossDef = "Годен";
                    }

                    if (longACount > 0)
                    {
                        longDef = "Брак зоны: " + CompressString(longA, longACount); //longDef;
                    } 
                    else
                    {
                        longDef = "Годен";
                    }

                    if (thickCount > 0)
                    {
                        thickDef = "Брак зоны: " + CompressString(thick, thickCount); //thickDef;
                    }
                    else
                    {
                        thickDef = "Годен";
                    }

                    if (b2Count > 0)
                    {
                        strobeDef = "Брак зоны: " + CompressString(b2A, b2Count); //strobe bit 2;
                    }
                    else
                    {
                        strobeDef = "Годен";
                    }

                    packet.Cross = crossDef;
                    packet.Long = longDef;
                    packet.Thickness = thickDef;
                    packet.StrobeBit2 = strobeDef;

                    //-минимум максимум в зоне
                    int n0 = (int)reader[3];
                    int n1 = (int)reader[4];
                    int n2 = (int)reader[5];
                    int n3 = (int)reader[6];
                    int bytesCount = count * sizeof(double);
                    reader.GetBytes(7, 0, tmpBuf, 0, bytesCount);
                    for (int i = 0; i < count; ++i )
                    {
                        minBuf[i] = BitConverter.ToDouble(tmpBuf, i * sizeof(double));
                    }
                    double x = FindMin(n0, n1, n2, n3, minBuf, count);
                    match = regex.Match(x.ToString());
                    if (match.Success)
                    {
                        x = Convert.ToDouble(match.Groups[1].Value);
                    }
                    packet.MinThickness = x.ToString();
                    reader.GetBytes(8, 0, tmpBuf, 0, bytesCount);
                    for (int i = 0; i < count; ++i)
                    {
                        maxBuf[i] = BitConverter.ToDouble(tmpBuf, i * sizeof(double));
                    }
                    x = FindMax(n0, n1, n2, n3, maxBuf, count);
                    match = regex.Match(x.ToString());
                    if (match.Success)
                    {
                        x = Convert.ToDouble(match.Groups[1].Value);
                    }
                    packet.MaxThickness = x.ToString();
                    //-минимум максимум в зоне конец
                    t.Add(packet);
                }
            }
            return t;
        }
        private static string CompressString(byte[] d, int length)
        {
            int tail = 1;
            int counter = 0;
            string res = "";
            for (int i = 1; i <= length; ++i)
            {
                if (d[i - 1] == d[i])
                {
                    if(1 == d[i])++counter;
                }
                else
                {
                    if (1 == d[i - 1])
                    {
                        ++counter;
                        if (counter > 2)
                        {
                            res += " ";
                            res += (1 + tail).ToString();
                            res += "-";
                            res += i.ToString();
                            res += ",";                           
                        }
                        else
                        {
                            for (int j = 0; j < counter; ++j)
                            {
                                res += " ";
                                ++tail;
                                res += tail.ToString();
                                res += ",";
                            }
                        }
                    }
                    counter = 0;
                    tail = i;
                }
            }
            return res;
        }
    }
   
}
