using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using Microsoft.Win32;

//【XScript】C#脚本引擎v1.8源码（2015/2/9更新）
//http://www.newlifex.com/showtopic-369.aspx

/*
 * 脚本功能：
 * 1，修改编译时间
 * 2，生成bin文件，显示ROM大小
 */

namespace NewLife.Reflection
{
    public class ScriptEngine
    {
        static Int32 Main(String[] args)
        {
            var axf = GetAxf(args);
            // 修改编译时间
            if (!String.IsNullOrEmpty(axf) && WriteBuildTime(axf))
            {
                // 修改成功说明axf文件有更新，需要重新生成bin
                // 必须先找到Keil目录，否则没得玩
                var mdk = GetKeil();
                if (!String.IsNullOrEmpty(mdk) && Directory.Exists(mdk))
                {
                    var fromelf = mdk.CombinePath("ARMCC\\bin\\fromelf.exe");
                    var bin = Path.GetFileNameWithoutExtension(axf) + ".bin";
                    var bin2 = bin.GetFullPath();
                    //Process.Start(fromelf, String.Format("--bin {0} -o {1}", axf, bin2));
                    var p = new Process();
                    p.StartInfo.FileName = fromelf;
                    p.StartInfo.Arguments = String.Format("--bin {0} -o {1}", axf, bin2);
                    //p.StartInfo.CreateNoWindow = false;
                    p.StartInfo.UseShellExecute = false;
                    p.Start();
                    p.WaitForExit(5000);
                    var len = bin2.AsFile().Length;
                    Console.WriteLine("生成ROM文件{0}共{1:n0}字节/{2:n1}KB", bin, len, (Double)len / 1024);
                }
            }

            return 0;
        }

        static String GetAxf(String[] args)
        {
            var axf = args.FirstOrDefault(e => e.EndsWithIgnoreCase(".axf"));
            if (!String.IsNullOrEmpty(axf)) return axf.GetFullPath();

            // 搜索所有axf文件，找到最新的那一个
            var fis = Directory.GetFiles(".", "*.axf", SearchOption.AllDirectories);
            if (fis != null && fis.Length > 0)
            {
                // 按照修改时间降序的第一个
                return fis.OrderByDescending(e => e.AsFile().LastWriteTime).First().GetFullPath();
            }

            Console.WriteLine("未能从参数中找到输出文件.axf，请在命令行中使用参数#L");
            return null;
        }

        static Boolean WriteBuildTime(String axf)
        {
            // 修改编译时间
            var ft = "yyyy-MM-dd HHss";
            var sys = axf.GetFullPath();
            if (!File.Exists(sys)) return false;

            var dt = ft.GetBytes();
            // 查找时间字符串，写入真实时间
            using (var fs = File.Open(sys, FileMode.Open, FileAccess.ReadWrite))
            {
                if (fs.IndexOf(dt) > 0)
                {
                    fs.Position -= dt.Length;
                    var now = DateTime.Now.ToString(ft);
                    Console.WriteLine("找到编译时间的位置0x{08}，准备写入编译时间{1}", fs.Position, now);
                    fs.Write(now.GetBytes());

                    return true;
                }
            }

            return false;
        }

        static String GetKeil()
        {
            var reg = Registry.LocalMachine.OpenSubKey("Software\\Keil\\Products\\MDK");
            if (reg == null) return null;

            return reg.GetValue("Path") + "";
        }
    }
}