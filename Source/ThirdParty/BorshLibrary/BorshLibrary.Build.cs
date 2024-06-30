using System.IO;
using System.Diagnostics;
using UnrealBuildTool;

public class BorshLibrary : ModuleRules
{
	static readonly string RepoUrl = "https://github.com/etodanik/borsh-cpp20";
	static readonly string RepoTag = "main";
	readonly string IntermediatePath;
	readonly string BinariesPath;
	readonly string BuildPath; 
	readonly string RepoPath;
	
	public BorshLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		IntermediatePath = Path.Combine(ModuleDirectory, "Intermediate");
		BinariesPath = Path.Combine(ModuleDirectory, "Binaries", Target.Platform.ToString());
		BuildPath = Path.Combine(BinariesPath, Target.Architecture.ToString());
		RepoPath = Path.Combine(IntermediatePath, "borsh-cpp20");
		string includePath = Path.Combine(RepoPath, "include");
		
		Type = ModuleType.External;
		PublicSystemIncludePaths.Add("$(ModuleDir)/Public");
		
		if (!Directory.Exists(RepoPath))
		{
			System.Console.WriteLine($"Directory does not exist: '{RepoPath}', cloning...");
			CloneRepo();
		}

		if (!Directory.Exists(BuildPath))
		{
			BuildRepo();
		}
		
		PublicIncludePaths.Add(includePath);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Link static library here
		}
		if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			// Link static library here
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			// Link static library here
		}
	}
	
	private void BuildRepo()
	{
		RunCommand("cmake", $"-DBORSH_BUILD_TESTS=ON -S \"{RepoPath}\" -B \"{BuildPath}\"", RepoPath);
		RunCommand("cmake", $"--build \"{BuildPath}\" --target borsh_test --config Release", RepoPath);
		
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			RunCommand(Path.Combine(BuildPath, "test", "Release", "borsh_test.exe"), BuildPath);
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			RunCommand(Path.Combine(BuildPath, "test", "borsh_test"), BuildPath);
		}
		// TODO: Check if all tests are indeed being run
	}

	private void CloneRepo()
	{
		if (!Directory.Exists(IntermediatePath))
		{
			Directory.CreateDirectory(IntermediatePath);
		}

		if (!Directory.Exists(RepoPath))
		{
			RunCommand("git", $"clone --depth 1 --branch \"{RepoTag}\" \"{RepoUrl}\" \"{RepoPath}\"");
		}
	}
	
	private void RunCommand(string command, string arguments, string workingDirectory = null)
	{
		System.Console.WriteLine($"Running command: {command} {arguments}");
		
		using (Process proc = new Process())
		{
			proc.StartInfo.FileName = command;
			proc.StartInfo.Arguments = arguments;
			proc.StartInfo.RedirectStandardOutput = true;
			proc.StartInfo.RedirectStandardError = true;
			proc.StartInfo.UseShellExecute = false;
			proc.StartInfo.CreateNoWindow = true;

			if (!string.IsNullOrEmpty(workingDirectory))
			{
				proc.StartInfo.WorkingDirectory = workingDirectory;
			}

			proc.OutputDataReceived += (sender, e) => 
			{
				if (e.Data != null)
				{
					System.Console.WriteLine(e.Data);
				}
			};

			proc.ErrorDataReceived += (sender, e) =>
			{
				if (e.Data != null)
				{
					System.Console.WriteLine("ERROR: " + e.Data);
				}
			};

			proc.Start();
			proc.BeginOutputReadLine();
			proc.BeginErrorReadLine();
			proc.WaitForExit();

			if (proc.ExitCode != 0)
			{
				throw new BuildException($"Command '{command} {arguments}' failed with exit code {proc.ExitCode}");
			}
		}
	}
}