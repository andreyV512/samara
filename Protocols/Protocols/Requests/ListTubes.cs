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
        static public IEnumerable<TubesView> Get(long protocols, string packet)
        {
            string query = "SELECT tt.NumberTube, tt.Date_Time"
               + " FROM [StoredBase].[dbo].[TubesTable] tt"
               + " JOIN [StoredBase].[dbo].[ProtocolsTable] pt"
               + " ON tt.IDProtocolsTable = pt.ID"
               + " WHERE pt.NumberPacket = @packet"
               + " ORDER BY tt.Date_Time";
            IList<TubesView> t = new List<TubesView>();
            using (SqlConnection connection = new SqlConnection(Properties.Settings.Default.StoredBaseConnectionString))
            {
                SqlCommand command = new SqlCommand(query, connection);

                command.Parameters.Add("@packet", SqlDbType.VarChar);
                command.Parameters["@packet"].Value = packet;

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
