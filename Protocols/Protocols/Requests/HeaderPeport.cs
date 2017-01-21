using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Protocols.Models;
/*
 *  public class UltrasonicControlProductModels
    {
        public int      ID               { get; set; }
        public string   Alloy            { get; set; }
        public string   DeliveryStatus   { get; set; }
        public string   NormativeDocument{ get; set; }
        public string   Gang             { get; set; }
        public string   ProductCodeNumber{ get; set; }
        public string   NumberPacket     { get; set; }
        public string   Standart         { get; set; }
        public int      Count            { get; set; }
        public DateTime TteTme           { get; set; }
        public string   Operator         { get; set; }
    }
 */
/*
 * SELECT TOP 1000 [ID]
      ,[Alloy]
      ,[DeliveryStatus]
      ,[NormativeDocument]
      ,[Gang]
      ,[ProductCodeNumber]
      ,[NumberPacket]
      ,[Standart]
  FROM [StoredBase].[dbo].[ProtocolsTable]
 * */
namespace Protocols.Requests
{
    class HeaderReport
    {
        public static UltrasonicControlProductModels Do(int id)
        {
            UltrasonicControlProductModels m = new UltrasonicControlProductModels();
            string query = "SELECT ,[Alloy],[DeliveryStatus],[NormativeDocument],[Gang],[ProductCodeNumber],[NumberPacket],[Standart]"
            + " FROM  [StoredBase].[dbo].[ProtocolsTable]";
            return m;
        }
    }
}
