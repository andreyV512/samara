using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Protocols.Models;
using System.Data.SqlClient;
using System.Data;

namespace Protocols.Requests
{
    public class RequestListTubes
    {
        static public IEnumerable<TubesView> Get(//long protocols
              string NumberPacket
            , string Alloy
            , string DeliveryStatus
            , string NormativeDocument
            , string Gang
            , string ProductCodeNumber           
            , string Standart)
        {
            string query = 
               // "SELECT tt.NumberTube, tt.Date_Time"
               //+ " FROM TubesTable tt"
               //+ " JOIN ProtocolsTable pt"
               //+ " ON tt.IDProtocolsTable = pt.ID"
               //+ " WHERE pt.NumberPacket = @packet"
               //+ " ORDER BY tt.Date_Time";

            "SELECT tt.NumberTube, tt.Date_Time"
              + " FROM [StoredBase0].[dbo].TubesTable tt"
              + " JOIN [StoredBase0].[dbo].ProtocolsTable pt"
              + " ON tt.IDProtocolsTable    = pt.ID"
              + " WHERE pt.NumberPacket     = @NumberPacket"          
              + " and pt.Alloy              = @Alloy"
              + " and pt.DeliveryStatus		= @DeliveryStatus"
              + " and pt.NormativeDocument	= @NormativeDocument"
              + " and pt.Gang				= @Gang"
			  + " and pt.ProductCodeNumber	= @ProductCodeNumber"
              + " and pt.Standart			= @Standart"
              + " ORDER BY tt.Date_Time"
                 ;
            IList<TubesView> t = new List<TubesView>();
            using (SqlConnection connection = new SqlConnection(Properties.Settings.Default.StoredBaseConnectionString))
            {
                SqlCommand command = new SqlCommand(query, connection);

                //command.Parameters.Add("@packet", SqlDbType.VarChar);
                 command.Parameters.Add("@NumberPacket", SqlDbType.VarChar);
                 command.Parameters.Add("@Alloy", SqlDbType.VarChar);
                 command.Parameters.Add("@DeliveryStatus", SqlDbType.VarChar);
                 command.Parameters.Add("@NormativeDocument", SqlDbType.VarChar);
                 command.Parameters.Add("@Gang", SqlDbType.VarChar);
                 command.Parameters.Add("@ProductCodeNumber", SqlDbType.VarChar);
                 command.Parameters.Add("@Standart", SqlDbType.VarChar);


                //command.Parameters["@packet"].Value = packet;
                command.Parameters["@NumberPacket"].Value = NumberPacket       ;
                command.Parameters["@Alloy"].Value = Alloy              ;
                command.Parameters["@DeliveryStatus"].Value = DeliveryStatus		;
                command.Parameters["@NormativeDocument"].Value = NormativeDocument	;
                command.Parameters["@Gang"].Value = Gang				;
                command.Parameters["@ProductCodeNumber"].Value = ProductCodeNumber	;
                command.Parameters["@Standart"].Value = Standart			;

                command.Connection.Open();
                SqlDataReader reader = command.ExecuteReader(CommandBehavior.SequentialAccess);
                
                while (reader.Read())
                {
                    TubesView r = new TubesView();
                    r.NumTube = (string)reader[0];
                    r.TteTme = (DateTime)reader[1];
                  
                    t.Add(r);
                }
            }
            return t;
        }
    }
}
