using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Protocols.Models;
using Protocols.Requests;
using System.Drawing.Printing;
using System.Reflection;
using Microsoft.Reporting.WinForms;

namespace Protocols
{
    public partial class ReportForm : Form
    {
        public ReportForm()
        {
            InitializeComponent();
            HiddenExportMenu("Excel");
            HiddenExportMenu("WORD");
        }
        
        public void ShowReport(int id, long numberProtocol, DateTime tdeTme, string num, int count, string Operator)
        {
            reportViewer1.LocalReport.DataSources.Clear();
            reportViewer1.LocalReport.ReportPath = "ProtocolReport.rdlc";
            reportViewer1.LocalReport.DisplayName = num;
            

            var header = UltrasonicControlProducts.HeaderProtocol(id, numberProtocol, tdeTme, count, Operator);

            Microsoft.Reporting.WinForms.ReportDataSource dataset1 = new Microsoft.Reporting.WinForms.ReportDataSource("DataSet1", header); // set the datasource
            reportViewer1.LocalReport.DataSources.Add(dataset1);

            var body = UltrasonicControlProducts.BodyProtocol(id);

            Microsoft.Reporting.WinForms.ReportDataSource dataset2 = new Microsoft.Reporting.WinForms.ReportDataSource("DataSet2", body); // set the datasource
            reportViewer1.LocalReport.DataSources.Add(dataset2);

            reportViewer1.LocalReport.Refresh();
            reportViewer1.RefreshReport();

            Show();
        }
        private void HiddenExportMenu(string strFormatName)
        {
            FieldInfo info;
            foreach (RenderingExtension extension in reportViewer1.LocalReport.ListRenderingExtensions())
            {
                if (extension.Name == strFormatName)
                {
                    info = extension.GetType().GetField("m_isVisible", BindingFlags.Instance | BindingFlags.NonPublic);
                    info.SetValue(extension, false);
                    break;
                }
            } 
        }
    }
}
