using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Protocols
{
    public partial class DuringPeriodForm : Form
    {
        public DateTime from, to;
        public DuringPeriodForm()
        {
            InitializeComponent();
        }

        private void Cancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void OkBtn_Click(object sender, EventArgs e)
        {
            from = dateTimePickerFrom.Value;
            to = dateTimePickerTo.Value;
            if (from < to)
            {
                Close();
            }
            else
            {
                MessageBox.Show("Дата введена не корректно");
            }
        }
    }
}
