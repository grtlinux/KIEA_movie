<%@ Language = Python %>
<%
import sys
sys.stdout.write = Response.Write

s = "������ �ڵ� ���� ����"
print s
l = s.split()
l.reverse()
print '<br>�ܾ� ������ �ٲٱ�<br>'
print ' '.join(l)
print 'Testing..'
%>
<br />

<script language="JavaScript">
document.write("Ŭ���̾�Ʈ�� �ڵ� ���� ����")
</script>
<br />

<script language="Python" runat="server">
print '�ٽ� ������ �ڵ�..'
</script>
