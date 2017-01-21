using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.SqlClient;
using System.Data;

namespace Protocols.Requests
{
    public class RemoveProtocol
    {
        protected static void RemoveProtocolsTable(int id)
        {
            string queryString =
               " DELETE t"
               + " FROM ProtocolsTable as t"
               + " WHERE t.ID =  @IDProtocols"
               ;
            using (SqlConnection connection = new SqlConnection(Properties.Settings.Default.StoredBaseConnectionString))
            {
                SqlCommand command = new SqlCommand(queryString, connection);

                command.Parameters.Add("@IDProtocols", SqlDbType.Int);
                command.Parameters["@IDProtocols"].Value = id;

                command.Connection.Open();
                command.ExecuteNonQuery();
            }
        }

        public static void Do(int id)
        {
            RemoveProtocolsTable(id);            
        }
    }
}
