<div class="login-form-wrapper large-12 small-12 columns">
	<p class="cust-login"><?php print render($intro_text); ?></p>
	<div class="fortified-login">
  <?php print drupal_render_children($form); ?>
  </div>
</div>